/*
 * Copyright (c) 2014, Julien Bernard
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include "finalizers.h"

#include <functional>

#include <mm/accessibility.h>
#include <mm/cutoff.h>
#include <mm/erosion_score.h>
#include <mm/logical_combine.h>
#include <mm/ratio.h>
#include <mm/reachability.h>
#include <mm/slope.h>

#include "exception.h"
#include "output.h"
#include "print.h"

namespace mm {

  namespace {

    class display_erosion_score {
    public:
      void operator()(const heightmap& map) const {
        double value = erosion_score()(map);
        print_indent();
        std::printf("\terosion score: " BEGIN_VALUE "%f" END_VALUE "\n", value);
      }
    };

    class playability {
    public:
      playability(reachability::size_type unit_size, reachability::size_type building_size, double unit_talus, double building_talus)
      : m_unit_size(unit_size)
      , m_building_size(building_size)
      , m_unit_talus(unit_talus)
      , m_building_talus(building_talus)
      {
      }

      void operator()(const heightmap& map) const {
        // erosion_score
        double erosion = erosion_score()(map);
        print_indent();
        std::printf("\terosion score: " BEGIN_VALUE "%f" END_VALUE "\n", erosion);

        auto slope_map = slope()(map);

        // unit_score
        auto unit_map = cutoff(m_unit_talus)(slope_map);
        output_planemap(unit_map, "unit1.pnm");
        unit_map = reachability(m_unit_size)(unit_map);
        output_planemap(unit_map, "unit2.pnm");
        unit_map = accessibility()(unit_map);
        output_planemap(unit_map, "unit3.pnm");
        auto unit_score = ratio()(unit_map);
        std::printf("\tunit score: " BEGIN_VALUE "%f" END_VALUE "\n", unit_score);

        // building score
        auto building_map = cutoff(m_building_talus)(slope_map);
        output_planemap(building_map, "building1.pnm");
        building_map = reachability(m_building_size)(building_map);
        output_planemap(building_map, "building2.pnm");
        building_map = logical_combine()(building_map, unit_map, std::logical_and<bool>());
        output_planemap(building_map, "building3.pnm");
        auto building_score = ratio()(building_map);
        std::printf("\tbuilding score: " BEGIN_VALUE "%f" END_VALUE "\n", building_score);

        // playability score
        auto playability_score = erosion * unit_score * building_score;
        std::printf("\tplayability score: " BEGIN_VALUE "%f" END_VALUE "\n", playability_score);
      }

    private:
      reachability::size_type m_unit_size;
      reachability::size_type m_building_size;
      double m_unit_talus;
      double m_building_talus;
    };

  }

  /*
   * Finalizers
   */

  static void null_finalizer(const heightmap& map) {
  }

  static finalizer_function get_erosion_score_finalizer(YAML::Node node, heightmap::size_type size) {
    return display_erosion_score();
  }

  static finalizer_function get_playability_finalizer(YAML::Node node, heightmap::size_type size) {
    auto nu_node = node["unit_size"];
    if (!nu_node) {
      throw bad_structure("mapmaker: missing 'unit_size' in 'playability' finalizer parameters");
    }
    auto nu = nu_node.as<reachability::size_type>();

    auto tu_node = node["unit_talus"];
    if (!tu_node) {
      throw bad_structure("mapmaker: missing 'unit_talus' in 'playability' finalizer parameters");
    }
    auto tu = tu_node.as<double>();

    auto nb_node = node["building_size"];
    if (!nb_node) {
      throw bad_structure("mapmaker: missing 'building_size' in 'playability' finalizer parameters");
    }
    auto nb = nb_node.as<reachability::size_type>();

    auto tb_node = node["building_talus"];
    if (!tb_node) {
      throw bad_structure("mapmaker: missing 'building_talus' in 'playability' finalizer parameters");
    }
    auto tb = tb_node.as<double>();

    return playability(nu, nb, tu / size, tb / size);
  }

  /*
   * API
   */
  finalizer_function get_finalizer(YAML::Node node, heightmap::size_type size) {
    auto name_node = node["name"];
    if (!name_node) {
      throw bad_structure("mapmaker: missing 'name' in finalizer definition");
    }
    auto name = name_node.as<std::string>();

    print_indent();
    std::printf(BEGIN_TYPE "finalizer" END_TYPE ": '" BEGIN_NAME "%s" END_NAME "'\n", name.c_str());

    auto parameters_node = node["parameters"];

    if (name == "erosion-score") {
      return get_erosion_score_finalizer(parameters_node, size);
    }

    if (name == "playability") {
      return get_playability_finalizer(parameters_node, size);
    }

    return null_finalizer;
  }

  void finalize(const heightmap& map, finalizer_function finalizer, YAML::Node node) {
    finalizer(map);
  }

}
