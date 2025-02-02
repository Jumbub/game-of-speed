# Game of Life (C++)

![tests](https://github.com/jumbub/game-of-life-cpp/actions/workflows/.github/workflows/tests.yaml/badge.svg)
![speed](https://user-images.githubusercontent.com/8686526/153600499-e4fd92c4-3537-4fe9-a288-2aca03f88077.svg)

Investigating C++ optimization techniques while building Game of Life.

- [Results of interesting findings](#results-of-interesting-findings)

- [Log of benchmark improvements](#log-of-benchmark-improvements)

- [Also check out a **JavaScript implementation**, with competitive performance](https://github.com/Jumbub/game-of-life-js)

[![The benchmark world](https://user-images.githubusercontent.com/8686526/153599957-85b5a9cb-e57d-4d02-aa6e-c794e7d18d04.gif)](https://youtu.be/16G4Dir5br8)

_(the above gif is the benchmark scenario; but because of video capture, it's slow as heck; [click for video](https://youtu.be/16G4Dir5br8))_

<br>

## Getting started

#### Running the app

`make`

#### Running the tests

`make test`

#### Running a benchmark

`make benchmark`

<br>

## Log of benchmark improvements

**Benchmark hardware:**
- i5-7600K @ 4.4GHz
- GTX 1080

**Benchmark scenario:**
- The benchmark is "time to compute 2000 generations; on a wrapping 2560x1440 sized board"
- The initial board is always the same, and contains
  - A [breeder](https://en.wikipedia.org/wiki/Breeder_(cellular_automaton)#:~:text=In%20cellular%20automata%20such%20as,copies%20of%20a%20tertiary%20pattern.) on the top half
  - Seeded random cells in the bottom left
  - Chess grid of 8x8 alive/dead cells in the bottom right

### Functioning prototype

Finalised the benchmark board.

[9227c6a55ede200a1b6fe827c93010963e704f3d](https://github.com/Jumbub/game-of-life-cpp/commit/9227c6a55ede200a1b6fe827c93010963e704f3d)

### 525% speed increase in logic

Switching from a 2D vector `std::vector<std::vector<bool>>` to a 1D array `std::unique_pr<new bool[]>`.

[9284f489604e814bb362fe5aa0c5e41ec5158edc](https://github.com/Jumbub/game-of-life-cpp/commit/9284f489604e814bb362fe5aa0c5e41ec5158edc)

### 100% speed increase in logic

Merging and inlining the "finding neighbour positions" and "counting neighbours" logic.

[f64f4adbf7c6f956d2edcff397483de44ccaabec](https://github.com/Jumbub/game-of-life-cpp/commit/f64f4adbf7c6f956d2edcff397483de44ccaabec)

### 250% speed increase in rendering

Rendering the board logic, rather than creating a list of pixels to render.

[cda613a50c4314d087152fb26cf222c76d5ea92d](https://github.com/Jumbub/game-of-life-cpp/commit/cda613a50c4314d087152fb26cf222c76d5ea92d)

### 83% speed increase in logic

Removing the shared\_ptr wrappers, speed > safety.

[73e6448fe2a021d9063bf50323b4b99cd551ee15](https://github.com/Jumbub/game-of-life-cpp/commit/73e6448fe2a021d9063bf50323b4b99cd551ee15)

### 115% speed increase in logic

Reduced the number of neighbour checks from 8 (all) to 3.

Achieved by having a "sliding window" of the neighbour counts for the "left", "middle" and "right" neighbouring columns. When the next cell computes it's neighbours it shifts the window over (so "middle" becomes "left") and only has to generate the new "right" column.

[01cce885bd57f1a5aa4d113a32f4413a61f826a5](https://github.com/Jumbub/game-of-life-cpp/commit/01cce885bd57f1a5aa4d113a32f4413a61f826a5)

### 88% speed increase in logic

Only compute the y levels for the above and below cells once per row.

[9e80c59830588b05ee29c754530fabf50f51a9e0](https://github.com/Jumbub/game-of-life-cpp/commit/9e80c59830588b05ee29c754530fabf50f51a9e0)

### ∞% speed increase in rendering

Segmented the rendering and computation into separate threads. _At this stage_ the rendering takes less time than the computation, so effectively I've reduced render time to 0.

[2619b4164bd2fca3fd8b30dca22cd83a75fe43b9](https://github.com/Jumbub/game-of-life-cpp/commit/2619b4164bd2fca3fd8b30dca22cd83a75fe43b9)

### THREADS% speed increase in logic

Finally gave in to the lazy performance improvement, throwing more threads at the problem.

[2f68783cc8a3d9bd7590e617b71a5a0df010af56](https://github.com/Jumbub/game-of-life-cpp/commit/2f68783cc8a3d9bd7590e617b71a5a0df010af56)

### 25% speed increase in logic

When converting values of `0`/`maxInt` to `0`/`1` we now use a mathematical expression `1 - (state + 1)` instead of a ternary.

[21bd48749570ac9c05663c661f60ce5ede1223ff](https://github.com/Jumbub/game-of-life-cpp/commit/21bd48749570ac9c05663c661f60ce5ede1223ff)

### 70% speed increase in logic

Insane free performance improvement by passing the optimization flag `-Ofast`.

[21761f93075a7aa26c314ae675b92df07c5f5892](https://github.com/Jumbub/game-of-life-cpp/commit/21761f93075a7aa26c314ae675b92df07c5f5892)

### 1% speed increase in logic

Enabling the compiler flag `-Wsign-conversion` to find and remove any unecessary signed to unsigned int conversions.

[86b3634383c6bd2d2f4104c441b2a2328f2bf6f8](https://github.com/Jumbub/game-of-life-cpp/commit/86b3634383c6bd2d2f4104c441b2a2328f2bf6f8)

### 15% speed increase in logic, for 100% speed decrease in rendering

For now I am going to focus on the logic performance instead of the main loop performance. The first step towards that is removing the "render" centric logic which was storing cells as "pixel friendly" values.

If at some point I can figure out how to tell SDL to render 0/1 byte values, I will gladly do so, and the render performance will be restored.

[b075a2f68ded65e3828d18e972e9a0fd0a24d596](https://github.com/Jumbub/game-of-life-cpp/commit/b075a2f68ded65e3828d18e972e9a0fd0a24d596)

### 50% speed increase overall

This speed increase is likely due to the fact that it consumes more CPU which would otherwise be used by the render thread.

[46f00cb633e6186ad78cf3f679dd82dd0414f3c3](https://github.com/Jumbub/game-of-life-cpp/commit/46f00cb633e6186ad78cf3f679dd82dd0414f3c3)

### 50% speed increase overall

This speed increase is primarily from changing the render limit to 30fps, but also changing the thread count to 12.

I'm not entirely sure why this thread count is more performant, but it's pretty evident in the results: [benchmark\_full.txt](https://github.com/Jumbub/game-of-life-cpp/blob/ff5d78d8e77fc307229be5448c1370b9ab70bb19/results/benchmark_full.txt)

[ff5d78d8e77fc307229be5448c1370b9ab70bb19](https://github.com/Jumbub/game-of-life-cpp/commit/ff5d78d8e77fc307229be5448c1370b9ab70bb19)

### 7% speed increase overall

Moved to SDLs Surface based rendering, instead of texture rendering.

Inpiration for change: [https://github.com/eabrash/game-of-life](https://github.com/eabrash/game-of-life)

[a81d839cc3d99b3d7293040a3e36cba7a46a33fe](https://github.com/Jumbub/game-of-life-cpp/commit/a81d839cc3d99b3d7293040a3e36cba7a46a33fe)

### 100% speed increase in logic, 75% speed increase overall

Added a padding to the game board, removing the need to do any modulus operations while computing x and y positions.

Inpiration for change: [https://www.youtube.com/watch?v=ndAfWKmKF34](https://www.youtube.com/watch?v=ndAfWKmKF34)

[90e4b104e9ea92222a562a63d9183fc3df6919f8](https://github.com/Jumbub/game-of-life-cpp/commit/90e4b104e9ea92222a562a63d9183fc3df6919f8)

### 37% speed increase overall

Move to a smaller data type to allow better CPU caching.

I did not realise how much of a difference it would make.

[644b7a9f91951d203f999374105ce20069a32c4b](https://github.com/Jumbub/game-of-life-cpp/commit/644b7a9f91951d203f999374105ce20069a32c4b)

### 20% speed increase overall

By leveraging the ability to read 8 cell states in one read cycle using `*reinterpret_cast<uint64_t*>(cells)`, we can perform some smarter skip logic.

If we read the 8 cells above, adjacent and below and find that we have 0 total alive cells, we can skip ahead 6 positions.

[67762ed7fbb419eea90d763bd8cbe0fe4596f5c1](https://github.com/Jumbub/game-of-life-cpp/commit/67762ed7fbb419eea90d763bd8cbe0fe4596f5c1)

### 6.5% speed increase overall

By replacing the system level mutex (std::mutex) with a more simple "request worker pause" spin lock.

We don't need a complex mutex when the communication is tied from a main thread to a single worker thread.

[ed7b50d50eb512cd558e1c7c6ac22e311d0018c5](https://github.com/Jumbub/game-of-life-cpp/commit/ed7b50d50eb512cd558e1c7c6ac22e311d0018c5)

### 15% speed increase overall

By replacing the primitive multi-branching cell state calculation with a lookup table.

[4f54fa8..41067a2](https://github.com/jumbub/game-of-life-cpp/compare/4f54fa8..41067a2)

### 5% speed increase overall

By using the Intel compiler.

Intel(R) C++ Compiler Classic 2021.5.0

[b25f071659e718dfce5e8ad0a832a44267a8970d](https://github.com/Jumbub/game-of-life-cpp/commit/b25f071659e718dfce5e8ad0a832a44267a8970d)

### 2% speed increase overall

By moving to the new Intel compiler.

Intel(R) oneAPI DPC++/C++ Compiler 2022.0.0

[ef55d32d9d77edd43c91bff7eba516ba7fa1e4a1](https://github.com/Jumbub/game-of-life-cpp/commit/ef55d32d9d77edd43c91bff7eba516ba7fa1e4a1)

### 18% speed increase overall

By creating a job pool, so that faster threads who previously sat idle could now do more actual work.

[d350d8bf8b49191952a72b7cb74fa694b5c289c3](https://github.com/Jumbub/game-of-life-cpp/commit/d350d8bf8b49191952a72b7cb74fa694b5c289c3)

### 20% speed increase overall

By fitting 2 "skip" bits into a single byte.

It turns out that adding more than 2 (ie. 4 or 8), actually decreased performance.

With 8 skip bits in 1 byte, I was only getting 1.43s; which was actually slower than 1 bit per byte (1.37s).

[07ca8c52e0397edf7dd60e864a479e1ebb10b060](https://github.com/Jumbub/game-of-life-cpp/commit/07ca8c52e0397edf7dd60e864a479e1ebb10b060)

<br>

## Results of interesting findings

Bear in mind these findings were made on **my** computer(details of which are [here](#log-of-benchmark-improvements)), and may not be applicable to your machine or code.

### Leveraging the speed of sequential memory access (+10% speed)

Commit [here.](https://github.com/Jumbub/game-of-life-cpp/commit/234acceeb7817f8da738f327afa7a18eaa5806ac)

**Context:**
- To calculate a new cell state, you need to count the number of alive neighbours
- To count the number of alive neighbours, you need to retrieve their states from memory
- The memory is stored in a flat array of size `width*height`

**Base code:**
- Keeps track of 3 offsets to use on the `input` array

```
unsigned int yAboveBase = 0;
unsigned int yBelowBase = 0;
unsigned int yBase = 0;

neighbours[2] = input[yBelowBase + nextX] + input[yBase + nextX] + input[yAboveBase + nextX];
```

**10% faster code:**
- Creates 3 arrays which have their base offsets assigned to the `0` index

```
unsigned int *neighboursBelow neighboursBelow = &input[lastYBase];
unsigned int *neighboursMiddle neighboursMiddle = &input[middleYBase];
unsigned int *neighboursAbove neighboursAbove = &input[nextYBase];

neighbours[2] = neighboursBelow[nextX] + neighboursMiddle[nextX] + neighboursAbove[nextX];
```

I _assume_ the increase in performance is through the change to access memory being sequentially.

### Compiler optimizations != CPU optimizations

Commit [here.](https://github.com/Jumbub/game-of-life-cpp/commit/38412c067a9d70cbad8ed20d9a693cdb9074be0c)

I had initially enabled the "fastest" compiler optimization flag in [this](https://github.com/Jumbub/game-of-life-cpp/commit/21761f93075a7aa26c314ae675b92df07c5f5892) commit, with the assumption that tangible improvements in performance could only be made by making changes to the code (in this case by "optimizing" it).

What I have seen is that some "optimizations" in code may actually negatively impact performance because it limits the CPUs ability to perform _it's_ own runtime optimizations. It is my assumption that in the case of this code base, leaving in the extra code as "hints" to the CPU is actually allowing it to improve internal branch prediction and caching strategies.

Here are **my** results for each optimization flag provided by gcc:

```
-Ofast : 4.42ms (2nd)
-O3    : 4.54ms (4th)
-O2    : 4.52ms (3rd)
-O1    : 4.28ms (the best)
-O0    : 8.33ms (6th)
-Os    : 4.65ms (5th)
```

### Which `0`/`MAX_INT` to `0`/`1` conversion is faster?

Code at the time of this experiment [here.](https://github.com/Jumbub/game-of-life-cpp/commit/bc10bbc88ff9c17a27b0dcff030bf221487ca893#diff-4d7b1c4aec6e6dda9ec986006afcec04363acf48f874a0e1c1caa2fff15481d1)

```
// the best
inline const auto math(unsigned int n) {
  return (1 - (n + 1));
}

// 3.3% slower
inline const auto mask(unsigned int n) {
  return n & 1;
}

// 3.3% slower
inline const auto shift(unsigned int n) {
  return n >> 31;
}

// 30% slower
inline const auto bool(unsigned int n) {
  return n == UINT32_MAX;
}
```

After taking a look at the assembly, it turns out the first function compiles to the [_neg_](https://www.ibm.com/docs/it/aix/7.2?topic=set-neg-negate-instruction) instruction as opposed to the _and_ or _shift_ instructions. I guess _neg_ is just an easier computation.

### Which `if` condition is faster?

Code at the time of this experiment [here.](https://github.com/Jumbub/game-of-life-cpp/tree/44a384ece1a2376c991735c3ab4802b44ba15b8c)

```
// 4.51ms: the best
if (currentStateBool && (totalNeighbours < 2 || totalNeighbours > 3))
  output[i] = DEAD;
else if (!currentStateBool && totalNeighbours == 3)
  output[i] = ALIVE;
else
  output[i] = currentStateBool;

// 4.89ms: 8.5% slower
if (totalNeighbours == 3)
  output[i] = ALIVE;
else if (totalNeighbours < 2 || totalNeighbours > 3)
  output[i] = DEAD;
else
  output[i] = currentStateBool;

// 5.07ms: 12.5% slower
if (totalNeighbours < 2 || totalNeighbours > 3)
  output[i] = DEAD;
else if (totalNeighbours == 3)
  output[i] = ALIVE;
else
  output[i] = currentStateBool;
```

I had assumed that the 3rd case would actually be the fastest, because _most_ cells are dead and have less than 2 neighbours.

My only vague idea about how this first solution could be faster is that it is able to do better branch prediction.

### Un-packing FOR the win

Commit on which experiment was conducted: [39d49374ee69aba70c7ab093397e131ecbb80665](https://github.com/Jumbub/game-of-life-cpp/tree/39d49374ee69aba70c7ab093397e131ecbb80665)

This:

```
output[i] = DEAD;
output[i + 1] = DEAD;
output[i + 2] = DEAD;
output[i + 3] = DEAD;
output[i + 4] = DEAD;
output[i + 5] = DEAD;
```
```
BM_Main/iterations:1/repeats:3/process_time/real_time              2.17 s          5.98 s             1
BM_Main/iterations:1/repeats:3/process_time/real_time              2.14 s          5.96 s             1
BM_Main/iterations:1/repeats:3/process_time/real_time              2.18 s          5.99 s             1
BM_Main/iterations:1/repeats:3/process_time/real_time_mean         2.16 s          5.98 s             3
BM_Main/iterations:1/repeats:3/process_time/real_time_median       2.17 s          5.98 s             3
BM_Main/iterations:1/repeats:3/process_time/real_time_stddev      0.019 s         0.014 s             3
```

is way faster than this:

```
for (uint j = 0; j < 6; j++) {
  output[i + j] = DEAD;
}
```
```
BM_Main/iterations:1/repeats:3/process_time/real_time              2.37 s          6.74 s             1
BM_Main/iterations:1/repeats:3/process_time/real_time              2.41 s          6.77 s             1
BM_Main/iterations:1/repeats:3/process_time/real_time              2.41 s          6.76 s             1
BM_Main/iterations:1/repeats:3/process_time/real_time_mean         2.40 s          6.76 s             3
BM_Main/iterations:1/repeats:3/process_time/real_time_median       2.41 s          6.76 s             3
BM_Main/iterations:1/repeats:3/process_time/real_time_stddev      0.019 s         0.017 s             3
```

### Struct memory layouts are slightly important

So, I had this extra pointer `renderRaw` in my `Board` struct which was legacy from previous render strategies. I deleted the pointer and it's associated `malloc` in [this commit](https://github.com/Jumbub/game-of-life-cpp/commit/e79b2f7a01bda0cf28925a4be90fb84cdabe3515), only to find out, it causes a 5% decrease in the overall performance of the app.

<br>

## References

Very good utility for visualising assembly instructions of your program:
[https://godbolt.org/](https://godbolt.org/)

Great util for generating binary image files:
[https://www.dcode.fr/binary-image](https://www.dcode.fr/binary-image)

Reference for alternative computation/render strategies:
[https://github.com/eabrash/game-of-life](https://github.com/eabrash/game-of-life)
