//====GLOBALS=====
const { mat4, mat3, vec3 } = glMatrix; 
const canvas = document.getElementById("glCanvas");
const gl = canvas.getContext("webgl2");
var vao; 
var projMatrix;
var mvpMatrixLoc;
var angleX = 90, angleY = 30;



function createShader(gl, type, source) {
  var shader = gl.createShader(type);
  gl.shaderSource(shader, source);
  gl.compileShader(shader);
  var success = gl.getShaderParameter(shader, gl.COMPILE_STATUS);
  if (success) {
    return shader;
  }
  console.log(gl.getShaderInfoLog(shader));
  gl.deleteShader(shader);
}

function createProgram(gl, vertexShader, fragmentShader) {
  var program = gl.createProgram();
  gl.attachShader(program, vertexShader);
  gl.attachShader(program, fragmentShader);
  gl.linkProgram(program);
  var success = gl.getProgramParameter(program, gl.LINK_STATUS);
  if (success) {
    return program;
  } 
  console.log(gl.getProgramInfoLog(program));
  gl.deleteProgram(program);
}

function main() {

  if(!gl) {
	  alert("Error enabling WebGL context!");
	  return;
  } 
  canvas.width = canvas.clientWidth;
  canvas.height = canvas.clientHeight;
  gl.viewport(0, 0, canvas.width, canvas.height);
  gl.clearColor(0.5, 1.0, 1.0, 1.0);
  gl.enable(gl.DEPTH_TEST);
  
  const vertexData = [
    5.0, 5.0, 5.0,  // Front
    5.0, -5.0, 5.0,
    -5.0, 5.0, 5.0,
    -5.0, 5.0, 5.0,
    5.0, -5.0, 5.0,
    -5.0, -5.0, 5.0,

    -5.0, 5.0, 5.0,   // Left
    -5.0, -5.0, 5.0,
    -5.0, 5.0, -5.0,
    -5.0, 5.0, -5.0,
    -5.0, -5.0, 5.0,
    -5.0, -5.0, -5.0,

    -5.0, 5.0, -5.0,    // Back
    -5.0, -5.0, -5.0,
    5.0, 5.0, -5.0,
    5.0, 5.0, -5.0,
    -5.0, -5.0, -5.0,
    5.0, -5.0, -5.0,

    5.0, 5.0, -5.0,     // Right
    5.0, -5.0, -5.0,
    5.0, 5.0, 5.0,
    5.0, 5.0, 5.0,
    5.0, -5.0, 5.0,
    5.0, -5.0, -5.0,

    5.0, 5.0, 5.0,    // Top
    5.0, 5.0, -5.0,
    -5.0, 5.0, 5.0,
    -5.0, 5.0, 5.0,
    5.0, 5.0, -5.0,
    -5.0, 5.0, -5.0,

    5.0, -5.0, 5.0,    // Bottom
    5.0, -5.0, -5.0,
    -5.0, -5.0, 5.0,
    -5.0, -5.0, 5.0,
    5.0, -5.0, -5.0,
    -5.0, -5.0, -5.0,
   ];   
  
  const colorData = [
    0, 0, 1,  // Front
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
	0, 0, 1,
    0, 0, 1,	

    1, 1, 0,   // Left
    1, 1, 0,
	1, 1, 0,
	1, 1, 0,
	1, 1, 0,
	1, 1, 0,
 
    0, 1, 1,    // Back
    0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

    1, 0, 0,     // Right
    1, 0, 0,
	1, 0, 0,
	1, 0, 0,
	1, 0, 0,
	1, 0, 0,

    0, 1, 0,    // Top
    0, 1, 0,
	0, 1, 0,
	0, 1, 0,
	0, 1, 0,
	0, 1, 0,

    1, 0, 1,    // Bottom
    1, 0, 1,
	1, 0, 1,
	1, 0, 1,
	1, 0, 1,
	1, 0, 1
   ];   

  var vsSource = document.getElementById("vertexShader").text;
  var fsSource = document.getElementById("fragmentShader").text;

  var vertexShader = createShader(gl, gl.VERTEX_SHADER, vsSource);
  var fragmentShader = createShader(gl, gl.FRAGMENT_SHADER, fsSource);

  var program = createProgram(gl, vertexShader, fragmentShader) 
  gl.useProgram(program);

  vao = gl.createVertexArray();
  gl.bindVertexArray(vao);
  
  const positionBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertexData), gl.STATIC_DRAW);
  
  const colorBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, colorBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colorData), gl.STATIC_DRAW);

  const positionLoc = gl.getAttribLocation(program, 'position');
  gl.enableVertexAttribArray(positionLoc);
  gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
  gl.vertexAttribPointer(positionLoc, 3, gl.FLOAT, false, 0, 0);
  
  const colorLoc = gl.getAttribLocation(program, 'color');
  gl.enableVertexAttribArray(colorLoc);
  gl.bindBuffer(gl.ARRAY_BUFFER, colorBuffer);
  gl.vertexAttribPointer(colorLoc, 3, gl.FLOAT, false, 0, 0);
  
  mvpMatrixLoc = gl.getUniformLocation(program, "mvpMatrix")
  projMatrix = mat4.create();
  mat4.perspective(projMatrix, 75 * Math.PI/180, 1, 10, 100);

 display();
}

function display() {  

  const modelMatrix = mat4.create();
  mat4.rotateY(modelMatrix, modelMatrix, angleY*Math.PI/180.0);
  mat4.rotateX(modelMatrix, modelMatrix, angleX*Math.PI/180.0);

  const viewMatrix = mat4.create();  //Camera
  mat4.lookAt(viewMatrix, [0, 0, 20], [0,0,0], [0,1,0]);

  const mvMatrix = mat4.create(); 
  mat4.multiply(mvMatrix, viewMatrix, modelMatrix);
  const mvpMatrix = mat4.create(); 
  mat4.multiply(mvpMatrix, projMatrix, mvMatrix);
  
  gl.uniformMatrix4fv(mvpMatrixLoc, false, mvpMatrix);  

  gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);   
  gl.bindVertexArray(vao);
  gl.drawArrays(gl.TRIANGLES, 0, 36);
  
  gl.bindVertexArray(null);
  gl.bindBuffer(gl.ARRAY_BUFFER, null);
}


main();