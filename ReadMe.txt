    __          __                    ______            _          
   / /   __  __/ /____  ________     / ____/___  ____ _(_)___  ___ 
  / /   / / / / __/ _ \/ ___/ _ \   / __/ / __ \/ __ `/ / __ \/ _ \
 / /___/ /_/ / /_/  __/ /__/  __/  / /___/ / / / /_/ / / / / /  __/
/_____/\__,_/\__/\___/\___/\___/  /_____/_/ /_/\__, /_/_/ /_/\___/ 
                                              /____/               

By Dennis 'DeeFeS' Schmidt (2020)
For Howest - DAE | Programming 4

Unfortunately I didn't have time to implement everything I wanted. Including multithreading for the input loop, collision detection
and to provide async threads to load level etc. in the background.
I started with a lot of Unity in mind - designwise, but due to the amount of time I spent with the Overlord Engine, it became a bit of both.

From Unity I implemented the basic functions. (OnEnable, OnDisable, SetActive, FixedUpdate etc.)

From Overlord I used the Logger, but also implemented the physics structure in a physics scene.

A lot of my engine is based on templates and callbacks (EventSystem, Command-Structure, CollisionDetection) to give me the most customization options within the game.
It's also something that was inspired by the Unity EventSystem.

Due to time constraints I only started implementing a ResourceManager and the ScriptableObject structure from Unity, but it's not implemented, yet.

https://github.com/DeeFeS/LuteceEngine