# Mastering Game Development with Unreal Engine 4,Second Edition

<a href="https://www.packtpub.com/game-development/mastering-game-development-unreal-engine-4-second-edition?utm_source=github&utm_medium=repository&utm_campaign=9781788991445"><img src="https://www.packtpub.com/sites/default/files/9781788991445.png"  alt="Book Name" height="256px" align="right" ></a>

This is the code repository for [Mastering Game Development with Unreal Engine 4, Second Edition](https://www.packtpub.com/game-development/mastering-game-development-unreal-engine-4-second-edition?utm_source=github&utm_medium=repository&utm_campaign=9781788991445), published by Packt.

**Build high-performance AAA games with UE 4**

## What is this book about?
To make a basic combat game from scratch, you will quickly override existing UE4 classes, and add and implement simple C++ functions while running and building them. These are all discussed as a short summary for new developers and as a quick refresher for experienced developers. Next, you will build a combat player character with expanded controls, create logic for a character, swap weapons, attack and move, bridge over scene changes and transitions, retain data between scenes, and manage the scene-change process.

This book covers the following exciting features: <First 5 What you'll learn points>
* The fundamentals of a combat-based game that will let you build and work all other systems from the core gameplay: the input, inventory, A.I. enemies, U.I., and audio
* Manage performance tools and branching shaders based on platform capabilities in the Material Editor
* Explore scene or level transitions and management strategies
* Improve visuals using UE4 systems such as Volumetric Lightmaps, Precomputed Lighting, and Cutscenes
* Implement audio-to-animation timelines and trigger them from visual FX

If you feel this book is for you, get your [copy](https://www.amazon.com/dp/1788991443) today!

<a href="https://www.packtpub.com/?utm_source=github&utm_medium=banner&utm_campaign=GitHubBanner"><img src="https://raw.githubusercontent.com/PacktPublishing/GitHub/master/GitHub.png" 
alt="https://www.packtpub.com/" border="5" /></a>


## Instructions and Navigations
Please note, to build, you first need to right click the .uproject file and select your UE4 engine which will also generate the .sln file.

If you plan to build VS2017 project files all the time, I recommend making a batch file here with this line in it:

(Engine-install, like D:/Dev/UE4/)Engine/Binaries/DotNET/UnrealBuildTool.exe -projectfiles -project="(Project-install directory, i.e. the one this file lives in)/Mastering.uproject" -game -engine -progress -2017

Where you insert the path to your preferred engine directory and project directory, where they're specified. This can also be used from a command-prompt and guarantees that both your engine and project will be built as 2017 and not as VS2015 projects. Note, though, if you right click the project file and change to a new engine, or generate project files from the Explorer fly-out menu, this will rebuild both game and project file by default as VS2015 again! Something to be aware of for those with both versions installed who want to work in VS2017.

The code will look like the following:
```
/** Muzzle offset */
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
class USceneComponent* MuzzleLocation;
```

**Following is what you need for this book:**
This book is for game developers who want to build high-performance games with amazing UIs. Experience with C++ is required and some knowledge of working with Unreal Engine 4 would be an advantage.

With the following software and hardware list you can run all code files present in the book (Chapter 1-13).

### Software and Hardware List

| Chapter  | Software required                   | OS required                        |
| -------- | ------------------------------------| -----------------------------------|
| 1-13     | Microsoft Visual Studio 2015 & 2017 |  Windows, Mac OS X, and Linux (Any)|
|          | Community Edition                   |                                    |



We also provide a PDF file that has color images of the screenshots/diagrams used in this book. [Click here to download it](https://www.packtpub.com/sites/default/files/downloads/9781788991445_ColorImages.pdf).

### Related products <Other books you may enjoy>
* Godot Engine Game Development Projects [[Packt]](https://www.packtpub.com/game-development/godot-game-engine-projects) [[Amazon]](https://www.amazon.com/dp/1788831500)

* Mastering C++ Game Development [[Packt]](https://www.packtpub.com/game-development/mastering-c-game-development) [[Amazon]](https://www.amazon.com/dp/1788293770)

## Get to Know the Author
**Matt Edmonds**
Matt Edmonds has been a lover of games for as long as he can remember, and a professional maker of PC, console, and mobile games since summer 2000. Having graduated with a degree in physics and mathematics, and while doing graduate studies in computer engineering, it became clear all of Matt's free time was going toward learning the technical and creative skills to build amazing 3D games. After making games on his own for around a year, he finally got a break after showing his work making a real-time strategy game in an open source 3D engine with Surreal Software and has never looked back: he now leads teams in creating amazing titles across generations of hardware. With a love of Unreal and 7 years of professional experience with it now, this book is a testament to that passion.

### Suggestions and Feedback
[Click here](https://docs.google.com/forms/d/e/1FAIpQLSdy7dATC6QmEL81FIUuymZ0Wy9vH1jHkvpY57OiMeKGqib_Ow/viewform) if you have any feedback or suggestions.
