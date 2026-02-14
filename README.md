# CountryQuiz

## C++/SFML geography learn & quiz program

<img width="1728" height="1117" alt="Screenshot 2026-02-12 at 5 26 56 PM" src="https://github.com/user-attachments/assets/bb7b8bc2-a153-477a-a611-321c547a468c" />

### ABOUT THE PROJECT
This program is one of very few projects which I undertook more for the use of the end product than for the challenge of coding it (I wanted my children to have a geography learning game.)

The only interesting part of coding and designing the program was how to get clicks to know whether they were in bounds of a given country, and relatedly, how to change the colors of those countries dynamically based on the mode or quiz status. This might have been less of a challenge than it was, but I wasn't going to draw maps from scratch in a paint program, so I had to find a solution that utilized map images available on the internet.
<details>
<summary>How I did it</summary>
The overarching idea I had to effect both aforementioned goals was to use an algorithm essentially like the fill bucket in a paint program. However (without delving into needless complexity), for that to work, the region in question must have all its pixels of the exact same color value (of which there are nearly 17 million.) Most images you find are not made up of solid colors, even when they appear so: a region that just looks blue is likely a smattered mosaic of dozens of subtly different bluish color values.

Anyway, I wrote some temporary code just to transform the downloaded Internet map images into something I could use. I ran the maps through an algorithm that set every pixel in the whole image to white, unless it was within a certain range of numeric values near to black, which left me with an image of just the country borders. I took that to a paint program and bucket-filled each country with my own choice of colors, which were now pixel-identical within a given country. I loaded this map onto the screen, held the mouse cursor over each separate country in turn, and typed its name. With more temporary code, this wrote the name and the coordinates for this anchor point into a text file (for broken-up countries like Indonesia, Denmark, and numerous others, I had to create multiple anchor points.) 

With that, when the player clicked anywhere on screen, a "fill bucket" algorithm would start at the click point. If, traveling one by one to neighboring pixels of the exact same color, the algorithm reached a pixel that matched an anchor point recorded in the text file, the program would know that the click had indeed occurred in the bounds of the country associated with that anchor point. The anchor points could also be used to start an actual paint bucket algorithm to change their color to green or red based on whether answered correctly in the quiz.  
</details>  

In most of my games all the graphics are created by me, some programmatically and some as pixel art. Here, as mentioned, the initial map images were scavenged, albeit significantly altered by my code. The majority of sound effects that I use are from the public domain. 

### FILE DESCRIPTIONS
* **sfmlApp:**  Implements `main()` and the abstract app
* **state:**  Implements primary graphical elements, click detection, game logic
* **country:**  Small structs representing countries and continents
  
(From my "reusable modules" repo: https://github.com/johnnywz00/SFML-shared-headers)
* **jwz:**  C++ utility functions, #defines, shortcuts
* **jwzsfml:**  Like above, but SFML-specific
* **resourcemanager:**  Static class for accessing resource files globally
* **timedeventmanager:**  Manages fuses/daemons, delayed callbacks

### BUILDING INSTRUCTIONS
Ready-made program files are available on the Releases page of this repository, with versions for MacOS, Windows, and Linux. NO INSTALLATION NECESSARY: just download and double-click. If your OS isn't supported by the pre-made versions, or if you have other reasons for building from source:
- Clone this repository, and navigate to the root folder of that clone in a terminal window.
- Run:
<pre>
   cmake -B build
   cmake --build build --parallel
</pre>

