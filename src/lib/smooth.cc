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
#include <mm/smooth.h>

namespace mm {

  heightmap smooth::operator()(const heightmap& src) const {
    heightmap map(src);
    heightmap out(size_only, src);

    for (size_type k = 0; k < m_iterations; ++k) {

      for (heightmap::size_type x = 0; x < map.width(); ++x) {
        for (heightmap::size_type y = 0; y < map.height(); ++y) {
          double value = 0.0;
          size_type count = 0;

          for (int i = -1; i <= 1; ++i) {
            if (x == 0 && i == -1) {
              continue;
            }

            if (x == map.width() - 1 && i == 1) {
              continue;
            }

            for (int j = -1; j <= 1; ++j) {
              if (y == 0 && j == -1) {
                continue;
              }

              if (y == map.height() - 1 && j == 1) {
                continue;
              }

              value += map(x+i, y+j);
              count += 1;
            }
          }

          out(x, y) = value / count;
        }
      }

      map = out;
    }

    return out;
  }

}
