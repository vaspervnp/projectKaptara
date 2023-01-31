projectKaptara
=====

[Open this project in 8bitworkshop](http://8bitworkshop.com/redir.html?platform=cpc.6128&githubURL=https%3A%2F%2Fgithub.com%2Fvaspervnp%2FprojectKaptara&file=main.asm).

Game Premise
====
Around 3800 years ago, on the great island we now know as Crete, preparations for the great annual festival in Knossos were in full swing. The Keftiu, the people we now known as the Minoans, would thank the Mother Goddess and the other deities for the crop and the wealth granted to them in the previous year, and ask for their favor for the next one. 

Alas, the sacred bull, the star of the festival and the bull-leaping event, has fallen gravely ill. Now, you, the most capable of the bull-leapers, must travel across the island to collect the ingredients and implements the High Priestess needs to prepare the cure for the bull in time for the ceremonies. Your journey will take you through lavish palaces, mystical caves, lush forests, and dark mines, in an adventure full of challenges and obstacles!

Game Basics
====
- Genre: Platform
- Mode: Single-player
- Game Environment: Flip-screen
- Initial Release for: Amstrad CPC 6128
- Graphics: Mixed-mode (Mode 0 & Mode 1), up to 16 colors on-screen
- Aspect ratio(s):
  - 4:3 (overall)
  - 16:10 (action window)
- Sound: AY-3-8910 (initially)
- Controls: keyboard, joystick

Characters
====
- Player Character: A young female bull-leaper
- NPCs:
  - The High Priestess
  - Minoan Scribe
  - Phoenician Merchant
  - Egyptian Emissary
  - Sage
  - Townsman #1
  - Townsman #2
  - Townswoman #1
  - Townswoman #2
  - Alley Cat
- Monsters:
  - Snake
  - Scorpion
  - Wolf
  - Wild Boar
  - Spider
  - Bat
  - Ibex
  - Spider
  - Evil Eyes
  - Bird of Prey
- Enemies (Human Hostile Characters):
  - Bandit #1
  - Bandit #2
  - Achaean Mercenary

Game Locales
====
- Palace
- Town
- Harbor
- Forest
- Mountain
- Caves
- Mines

Player Character
====
- Actions:
  - Idle
  - Walk (left-right)
  - Jump
  - Climb
  - Crouch / Duck
  - Crouch walk / Crawl (left-right)
  - Enter door / passage
  - Collect item
  - Use Item
  - Use Weapon
  - Sustain Damage (lose energy)
  - Death
  - Replenish Energy

- Weapons:
  - Dagger
  - Slingshot

- Weapon Usage Method and Mechanics:
  - Dagger: Thrown straight and its trajectory is subject to gravity
  - Slingshot: Fires at an variable angle and follows a ballistic trajectory
  - The range of the dagger depends on how long the fire button / key is pressed before being released
    - Maximum dagger range: whole screen length
  - The range and angle of the slingshot's projectile depend on how long the fire button / key is pressed before being released
    - Maximum slingshot projectile range: whole screen length
    - Angle of slingshot projectile respective to horizontal axis: 0-45°
  - Weapons / projectiles thrown cannot kill anything in other screens
  - Weapons / projectiles thrown kill enemies instantly

- Movement (left-right) / Crouch:
  - Walking speed: 1 Player Character width/step
  - Crouch walk is 50% slower
  - Crouching / ducking allows the character to dodge projectiles flying above her waist

- Jump: 
  - Vertical
  - Diagonal (see Opera Soft's "Goody")
  - Jumping allows the character to dodge projectiles flying below her waist
  - Jump height depends on how long the relevant button / key is depressed before being released;
    - Basic jump: half Player Character height
    - High jump: 1 Player Character height

- Climb:
  - Up or Down
  - Climbing speed: Similar to walking speed
  - Player Character can stop and use a weapon while climbing
  - Player Character can jump from what she's climbing on

- Energy:
  - Three types of energy:
    - Health (Hit Points)
    - Food
    - Water
  - Rate of energy depletion as a function of time:
    - Health: N/A
    - Food: X%/min
    - Water: 0.5X%/min
  - Rate of energy replenishment as a function of collecting power-ups:
    - Health:
      - Health-specific item: replenish to 100%
      - Food: replenishes 25% of maximum Health
      - Water: replenishes 25% of maximum Health
    - Food:
      - Health-specific item: replenish 10% of maximum Food
      - Food: replenish to 100%
      - Water: replenish 10% of maximum Food
    - Water: 
      - Health-specific item: replenish 10% of maximum Water
      - Food: replenish 10% of maximum Water
      - Water: replenish to 100%



