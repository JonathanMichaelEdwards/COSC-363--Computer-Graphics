#version 330

in vec4 oColor;
out vec4 outputColor;


void main()
{
   vec4 black = vec4(1.0);
   
   // float color = oColor.x;

   // if (oColor.x < 0.2)
   //    outputColor = black;

   // if (oColor < 0.2) 
   outputColor = (outputColor == 0) ? black : black;

   outputColor = oColor.x;
}
