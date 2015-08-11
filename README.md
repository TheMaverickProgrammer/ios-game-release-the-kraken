# About ios-game-release-the-kraken
A game I wrote back in 2012 using C++ targetting iOS devices.
Under the folder `Gosu` are the files I wrote to wrap the [Gosu](http://libgosu.org) Cross-platform Multi-media library.
Under the folder `Logic` was the makings of a game-development framework I would later call *Jiwon* that went undeveloped due to college curriculums.

# Brief comment on Jiwon
See the `Logic` folder.

The framework was written to be **library agnostic**. Any multi-media library could substitute.
Hence the folder name *Logic*, it was bare-bones and lightweight to be used for 2D game.

*Re-lease the Kraken* was one of the few games being developed with the framework as a *proof-of-concept*.

The game-development framework was realized to be highly functional, effective, and too immature.
Due to college curriculums I lost focus of the project. This is what was left.

# About Re-lease the Kraken

Our loveable beasts from the netherworld<sup>TM</sup> want to lease a new sportscar!
-But they are broke.

As the Kraken and other loveable beast from the netherworld<sup>TM</sup> chase you,
collect money while avoid hitting things so that the lease can be approved!

# How to compile
This project was compiled with Apple's Xcode along with [Gosu](http://libgosu.org)'s binaries.
See Gosu's [installation guide](https://github.com/gosu/gosu/wiki)
This project **has been tested with Windows** and will compile with those binaries.

# Folder structure
Output using the terminal command `tree`.

```
.
├── README.md
└── source
    ├── Event
    │   ├── Event.h
    │   ├── EventListener.h
    │   ├── EventPublisher.h
    │   └── NullEventListenerException.h
    ├── Gosu
    │   ├── Core.cpp
    │   ├── Core.h
    │   ├── Dependencies.h
    │   ├── GosuEx_Shader.h
    │   ├── Resources.h
    │   └── pixelate.frag
    ├── Kraken
    │   ├── EntityGroup.cpp
    │   ├── EntityGroup.h
    │   ├── Factory.cpp
    │   ├── Factory.h
    │   ├── GameStates.h
    │   ├── Group.h
    │   ├── KrakenGame.cpp
    │   ├── KrakenGame.h
    │   ├── KrakenGameState.cpp
    │   ├── KrakenGameState.h
    │   ├── KrakenGameTiledMap.cpp
    │   ├── KrakenGameTiledMap.h
    │   ├── KrakenNormal.cpp
    │   ├── KrakenNormal.h
    │   ├── Player.cpp
    │   ├── Player.h
    │   ├── Predator.h
    │   ├── SquidEasy.cpp
    │   └── SquidEasy.h
    ├── Logic
    │   ├── Camera.cpp
    │   ├── Camera.h
    │   ├── Entity.h
    │   ├── InputConfigDevice.cpp
    │   ├── InputConfigDevice.h
    │   ├── LogicState.h
    │   ├── LogicStateMachine.h
    │   ├── Map
    │   │   ├── GenericTiledMap.cpp
    │   │   ├── GenericTiledMap.h
    │   │   ├── Map.h
    │   │   └── TiledQT
    │   │       ├── TiledQT_TMXFileParser.cpp
    │   │       ├── TiledQT_TMXFileParser.h
    │   │       ├── TiledQT_TiledMap.cpp
    │   │       └── TiledQT_TiledMap.h
    │   ├── Sequence.cpp
    │   ├── Sequence.h
    │   └── Trigger.h
    ├── Math
    │   ├── Geometry
    │   │   ├── Line.cpp
    │   │   └── Line.h
    │   ├── Vector2D.cpp
    │   └── Vector2D.h
    ├── icon.png
    └── main.cpp

9 directories, 54 files
```
