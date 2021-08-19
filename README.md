# **IronBellyTest**
A fun little programming test

Here’s the little programming test. I.. Yeah, I probably spent more time in research than what was necessary for this project. I read a few articles and watched/listened to a few GDC presentations around FPS games and how they handle shooting, even though I only applied screen-space calculated shots to the project (Is this the standard way people do that? Most of what I could find revolved around level design). 

In case you’re interested in the material:
- http://web.cs.wpi.edu/~claypool/papers/fr/fulltext.pdf
- https://www.gamasutra.com/view/news/170721/Firstperson_shooter_design_What_to_save_and_what_to_frag.php
- http://www.aydinafzoud.com/game-design--analysis.html

Anyway, The first expected mechanic is what I spent most of the time on, even though the end-result solution was pretty simple, as I don’t have much experience with FPS programming and there is apparently a lot to learn!

## **The set backs:**

I always like to point out where I’ve fallen!
As I mentioned; I have limited networking experience in Unreal and the docs on Unreal tend to always be lacking for C++ (I usually read the BP docs and stumbe around from there). The most useful (and skimpy) pages I found on this topic were:
- https://docs.unrealengine.com/4.26/en-US/InteractiveExperiences/Networking/Actors/RPCs/
- https://docs.unrealengine.com/4.26/en-US/InteractiveExperiences/Networking/Actors/Properties/

So, I fired up an old project made back on Unreal 4.18, when I took the two Tom Looman Udemy courses to fill in the gaps, but I made it through!.

I also decided to grab the latest stable Unreal 4.26.2 for this project and have been fighting my way through this bug: 
- https://answers.unrealengine.com/questions/1009277/ue4-editor-flickersgoes-black.html

It took awhile of trying different driver versions, but oddly THIS was the fix:
- https://nvidia.custhelp.com/app/answers/detail/a_id/5157/~/after-updating-to-nvidia-game-ready-driver-461.09-or-newer%2C-some-desktop-apps

Well, I guess that’s it! Thanks for the opportunity and your time in reviewing this!
