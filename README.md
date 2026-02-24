# Beier-Neely Image Morphing

## Overview
This project implements the **Beier-Neely image morphing algorithm**, a feature-based warping technique for smooth transformation between two images.
This work was completed as part of a **Computer Graphics** class under **Crane He Chen** at **Northeastern University**, and the overall project/codebase was course-based rather than entirely built from scratch by me.

## Approach
The morphing pipeline has three steps:

1. **Feature Line Interpolation**: line pairs in source and destination images are interpolated for each time step.
2. **Warping**: both images are deformed toward the intermediate structure.
3. **Cross Dissolve**: warped images are blended to generate each final frame.

## Key Functions
- `BeierNeelyMorph`: interpolates line segments, warps both images, and cross-dissolves.
- `warp`: computes per-pixel source coordinates from feature lines and samples with bilinear interpolation.
- `bilinearInterpolate`: smooths sampled pixel values.
- `CrossDissolve`: blends warped source and destination by timestep.

## Visual Results

### Rotation Output
![Rotation output](output/output1.gif)

```cpp
const Eigen::MatrixXd F_src = (Eigen::MatrixXd(1, 4) <<
    176, 83, 176, 232
).finished();

const Eigen::MatrixXd F_dest = (Eigen::MatrixXd(1, 4) <<
    149, 83, 21, 159
).finished();
```

### Translated Output
![Translated output](output/output2.gif)

```cpp
const Eigen::MatrixXd F_src = (Eigen::MatrixXd(3, 4) <<
    177, 68, 177, 216,
    280, 68, 280, 97,
    271, 126, 270, 155
).finished();

const Eigen::MatrixXd F_dest = (Eigen::MatrixXd(3, 4) <<
    68, 68, 68, 216,
    171, 67, 171, 97,
    162, 126, 162, 155
).finished();
```

### Translated + Scaled Output
![Translated scaled output](output/output3.gif)

```cpp
const Eigen::MatrixXd F_src = (Eigen::MatrixXd(5, 4) <<
    177, 68, 177, 216,
    280, 68, 280, 97,
    271, 126, 270, 155,
    211, 97, 211, 125,
    211, 155, 211, 216
).finished();

const Eigen::MatrixXd F_dest = (Eigen::MatrixXd(5, 4) <<
    70, 87, 70, 203,
    174, 87, 174, 111,
    165, 132, 165, 156,
    105, 111, 105, 132,
    105, 156, 105, 203
).finished();
```

### Scaled Output
![Scaled output](output/output4.gif)

```cpp
const Eigen::MatrixXd F_src = (Eigen::MatrixXd(5, 4) <<
    177, 68, 177, 216,
    280, 68, 280, 97,
    271, 126, 270, 155,
    211, 97, 211, 125,
    211, 155, 211, 216
).finished();

const Eigen::MatrixXd F_dest = (Eigen::MatrixXd(5, 4) <<
    77, 96, 77, 197,
    225, 96, 225, 116,
    212, 135, 212, 156,
    127, 117, 127, 135,
    127, 156, 127, 197
).finished();
```

## Art Contest Submission
I tried multiple images from my phone but ran into issues with some inputs, so I used a scene from my favorite animated movie, *The Mitchells vs. The Machines*.

### Feature Line Pairs
```cpp
const Eigen::MatrixXd F_src = (Eigen::MatrixXd(2, 4) <<
    103, 90, 133, 90,
    118, 114, 118, 205
).finished();

const Eigen::MatrixXd F_dest = (Eigen::MatrixXd(2, 4) <<
    97, 55, 141, 45,
    105, 71, 118, 144
).finished();
```

### Final Animation
![Art contest morphing animation](output/art%20contest%20output.gif)

MP4: [output/art contest.mp4](output/art%20contest.mp4)
