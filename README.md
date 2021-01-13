# 2DLightingDemo
The goal of this project is to host multiple point lights in a space to illuminate a texture and have shadows.
<img src="assets/demo.PNG" width="500"/>

Currently, the light is controlled using sliders on a side panel, and you can adjust the radius of the light.
I still need to add:
<ul>
  <li>Light controlled by mouse</li>
  <li>Light can change colour</li>
  <li>Placing a light spawns a new one</li>
  <li>Shadows</li>
</ul>

### Going forward:
Maybe the current implementation isnt the best, the colours don't look natural when blending together, since I'm using basic additive blending.
For 99% of things, this will do just fine for a 2D game that needs some lighting, especially if the effect is subtle.
I plan on revisiting this project since I have an idea of how to make it look better in my mind, maybe try to add the RGB values together (Red ontop of cyan or vice versa will produce white light, whereas in this implementation whatever is most recently drawn goes ontop.) and use opacity as a means to make stacking lights ontop of eachother brighter. In theory this will be more realistic, but each light will need its own render pass in order to have its RGBA values added together with the previous light. Shadows would make this even more complicated and slower, each light would be pretty expensive to draw. If there are no shadows, then clearly it would be cheap and fast to add since everything can be done in one shader with one draw call by just passing the light position in as a uniform array of vec2.
