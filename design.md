projectKaptara
====
[Open this project in 8bitworkshop](http://8bitworkshop.com/redir.html?platform=cpc.6128&githubURL=https%3A%2F%2Fgithub.com%2Fvaspervnp%2FprojectKaptara&file=main.asm).

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

Game Setting & Premise
====
- __Setting:__ Bronze Age Crete, referred to in contemporary Syrian cuneiform tablets as *Kaptara*, hence the project's name.
- __Premise:__ Collect items and return them to a "home" position.

Characters
====
- __Player Character:__ A young female bull-leaper
- __NPCs:__
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
- __Monsters:__
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
- __Enemies (Human Hostile Characters):__
  - Bandit #1
  - Bandit #2
  - Achaean Mercenary

Game Locales
====
- Palace of Knossos
- Town
- Harbor
- Forest
- Mountain
- Caves
- Mines

Player Character
====
- Size relevant to Action Window height:
  - Approximately 12,5-13% of Action Window height (similarly to _Goody_ and _Livingstone Supongo_ by Opera Soft)

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
  - Slingshot: Fires at a variable angle and follows an optimal ballistic trajectory
  - The range of the dagger depends on how long the fire button / key is pressed before being released
    - Maximum dagger range: whole screen length
    - The dagger can kill two or more targets if all of them are in its path
  - The range and angle of the slingshot's projectile depend on how long the fire button / key is pressed before being released
    - Maximum slingshot projectile range: whole screen length
    - Angle of slingshot projectile respective to horizontal axis: 45° (for optimal ballistic trajectory)
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
  - Jump height: 1 Player Character height 

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
    - Food: 5%/min
    - Water: 5%/min
  - Rate of energy replenishment as a function of collecting power-ups:
    - Health:
      - Health-specific item: replenish to 100%
      - Food: replenishes 25% of maximum Health
      - Water: replenishes 25% of maximum Health
    - Food:
      - Health-specific item: replenishes 10% of maximum Food
      - Food: replenish to 100%
      - Water: replenishes 10% of maximum Food
    - Water: 
      - Health-specific item: replenishes 10% of maximum Water
      - Food: replenishes 10% of maximum Water
      - Water: replenish to 100%

