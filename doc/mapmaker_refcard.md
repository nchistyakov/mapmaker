# mapmaker

This document is a (sort of) reference card for `mapmaker`.

## Seed

Parameters:

* `seed`: a seed to generate the map (optional)

## Output

Generators and modifiers can have an `ouput`.

Parameters:

* `output`: the file to generate in [Netpbm format](http://en.wikipedia.org/wiki/Netpbm_format)
  * `type`: the type of output, one of: `colored`, `grayscale`
  * `parameters` : the parameters of the output (see below)
  * `filename`: the name of the file (with a `.pnm` extension)

### `colored`

Parameters:

* `sea_level`: the level of the sea (typically `0.5`)

Example:

```yml
  output:
    type: 'colored'
    parameters:
      sea_level: 0.5
    filename: 'map.pnm'
```

## Generators

Parameters:

* `name`: the name of the generator
* `parameters`: the parameters of the generator (see below)
* `size`: the size of the map (in pixels)
  * `width`: the width of the map (in pixels)
  * `height`: the height of the map (in pixels)
* `output`: see [Ouput](#output)

### `midpoint-displacement`

Parameters:

* `values`: initial values at the corner. Can be a number or an array of 4 numbers.

Examples:

```yml
generator:
  name: 'midpoint-displacement'
  parameters:
    values: 0
  size:
    width: 100
    height: 100
```

```yml
generator:
  name: 'midpoint-displacement'
  parameters:
    values: [0, 100, 200, 300]
  size:
    width: 200
    height: 200
```

### `diamond-square`

Parameters:

* `values`: initial values at the corner. Can be a number or an array of 4 numbers.

Examples:

```yml
generator:
  name: 'diamond-square'
  parameters:
    values: 0
  size:
    width: 100
    height: 100
```

```yml
generator:
  name: 'diamond-square'
  parameters:
    values: [0, 100, 200, 300]
  size:
    width: 200
    height: 200
```

### `fractal`

Parameters:

* `noise`: the noise function, one of: `value`, `gradient`, `simplex`, `cell`
* `noise_parameters`: the parameters for the noise function (see below)
* `scale`: the scale to apply to the noise function inputs (typically 1.0)
* `octaves`: the number of octaves (typically 10)
* `lacunarity`: the [lacunarity](http://en.wikipedia.org/wiki/Lacunarity) of the noise, i.e. the factor for the [frequency](http://en.wikipedia.org/wiki/Frequency) at each octave (typically `2.0`)
* `persistence`: the persistence of the noise, i.e. the factor for the [amplitude](http://en.wikipedia.org/wiki/Amplitude) (typically `0.5`)

#### `value` noise

Parameters:

* `curve`: a curve function, one of: `linear`, `cubic`, `quintic`, `cosine`

Example:

```yml
generator:
  name: 'fractal'
  parameters:
    noise: 'value'
    noise_parameters:
      curve: 'cubic'
    scale: 1.0
    octaves: 10
    lacunarity: 2.0
    persistence: 0.5
  size:
    width: 200
    height: 200
```

#### `gradient` noise

Parameters:

* `curve`: a curve function, one of: `linear`, `cubic`, `quintic`, `cosine`

Example:

```yml
generator:
  name: 'fractal'
  parameters:
    noise: 'gradient'
    noise_parameters:
      curve: 'quintic'
    scale: 1.0
    octaves: 10
    lacunarity: 2.0
    persistence: 0.5
  size:
    width: 200
    height: 200
```

#### `simplex` noise

Example:

```yml
generator:
  name: 'fractal'
  parameters:
    noise: 'simplex'
    scale: 1.0
    octaves: 10
    lacunarity: 2.0
    persistence: 0.5
  size:
    width: 200
    height: 200
```

#### `cell` noise

Parameters:

* `count`: the number of cells
* `distance`: the distance function, one of: `manhattan`, `euclidean`, `chebyshev`
* `coeffs`: the coefficients to apply to each cell, given as a non-empty array of numbers (typically `[-1, 1]`)

Example:

```yml
generator:
  name: 'fractal'
  parameters:
    noise: 'cell'
    noise_parameters:
      count: 32
      distance: 'euclidean'
      coeffs: [-1, 1]
    scale: 1.0
    octaves: 1
    lacunarity: 2.0
    persistence: 0.5
  size:
    width: 200
    height: 200
```

### `hills`

Parameters:

* `count`: the number of hills to generate (typically `100`)
* `radius_min`: the minimum radius of the hills, in fraction of the size of the map
* `radius_max`: the maximum radius of the hills, in fraction of the size of the map

Example:

```yml
generator:
  name: 'hills'
  parameters:
    count: 100
    radius_min: 0.3
    radius_max: 0.6
  size:
    width: 100
    height: 100
```

## Modifiers

Parameters:

* `name`: the name of the modifier
* `parameters`: the parameters of the modifier (see below)
* `output`: see [Ouput](#output)

### `thermal-erosion`

Parameters:

* `iterations`: the number of iterations of the erosion algorithm (typically `50`)
* `talus`: the relative talus difference (typically `4`)
* `fraction`: the fraction of material that goes down the talus (typically `0.5`)

Example:

```yml
modifiers:
  -
    name: 'thermal-erosion'
    parameters:
      iterations: 50
      talus: 4
      fraction: 0.5
```

### `hydraulic-erosion`

Parameters:

* `iterations`: the number of iterations of the erosion algorithm (typically `100`)
* `rain_amount`: the quantity of rain to add at each iteration (typically `0.01`)
* `solubility`: the proportion of material converted to sediment (typically `0.01`)
* `evaporation`: the proportion of water that evaporates (typically `0.5`)
* `capacity`: the proportion of sediment that goes back to material (typically `0.01`)

Example:

```yml
modifiers:
  -
    name: 'hydraulic-erosion'
    parameters:
      iterations: 100
      rain_amount: 0.01
      solubility: 0.01
      evaporation: 0.5
      capacity: 0.01
```

### `fast-erosion`

Parameters:

* `iterations`: the number of iterations of the erosion algorithm (typically `100`)
* `talus`: the relative talus difference (typically `8`)
* `fraction`: the fraction of material that goes down the talus (typically `0.5`)

Example:

```yml
modifiers:
  -
    name: 'fast-erosion'
    parameters:
      iterations: 100
      talus: 8
      fraction: 0.5
```

### `islandize`

Parameters:

* `border`: the portion of map border that will be turned into sea (typically `0.15`)

Example:

```yml
modifiers:
  -
    name: 'islandize'
    parameters:
      border: 0.15
```

### `gaussize`

Parameters:

* `spread`: the spread of the gaussian function (typically `0.3`)

Example:

```yml
modifiers:
  -
    name: 'gaussize'
    parameters:
      spread: 0.3
```

### `flatten`

Parameters:

* `factor`: the flatten factor (typically `2.0`)

Example:

```yml
modifiers:
  -
    name: 'flatten'
    parameters:
      factor: 2.0
```

### smooth

Parameters:

* `iterations`: the number of times the filter is applied

Example:

```yml
modifiers:
  -
    name: 'smooth'
    parameters:
      iterations: 10
```


### `intercept`

Parameters:

* `modifiers`: like global modifiers
* `finalizer`: like global finalizer

```yml
modifiers:
  -
    name: 'intercept'
    parameters:
      finalizer:
        name: 'erosion-score'

```

## Finalizers

### `erosion-score`

Example:

```yml
finalizer:
  name: 'erosion-score'
```

### `playability`

Parameters:

* `sea_level`: the level of the sea (typically `0.5`)
* `unit_size`: the size of units (typically `1`)
* `building_size`: the size of buildings (typically `9`)
* `unit_talus`: the relative talus difference that a unit can cross (typically `8`)
* `building_talus`: the relative talus difference that a building can be built on (typically `2`)
* `output_intermediates`: output the intermediate unit maps and building maps (typically `false`)

Example:

```yml
finalizer:
  name: 'playability'
  parameters:
    sea_level: 0.5
    unit_size: 1
    building_size: 9
    unit_talus: 8.0
    building_talus: 2.0
    output_intermediates: false
```
