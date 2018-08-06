#extension GL_ARB_gpu_shader5 : enable
uniform float zNear ; 
uniform float zFar ; 

const int MAX_MARCHING_STEPS = 256;
const float MIN_DIST = 0.0001;
const float MAX_DIST = 100.0;
const float EPSILON = 0.00001;

const vec3 lightPos=vec3(1.0,1.0,1.0);

//// This text will be replaced by the shape compiler before compiling the shader. 
REPLACE_WITH_UNIFORMS

vec3 sub(vec3 a, vec3 b)
{
        return a-b;
}

vec3 rotate(vec3 p, float angle)
{
        float c = cos(angle);
        float s = sin(angle);
        return vec3( p.x*c - p.y*s, p.x*s+p.y*c, p.z  );
}


vec3 add(vec3 a, vec3 b)
{
        return a+b;
}

vec3 mul(vec3 a, float b)
{
        return a*b;
}

vec3 div(vec3 a, float b)
{
        return a/b;
}



float mul(float a, float b)
{
        return a*b;
}

float div(float a, float b)
{
        return a/b;
}

float sdPlane( vec3 p, vec3 n, float w )
{
  // n must be normalized
  return dot(p,n.xyz) + w;
}

float sdBox( vec3 p, vec3 b )
{
  vec3 d = abs(p) - b;
  return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}

vec3 spaceTiling( vec3 p, vec3 c )
{
    return mod(p,c)-0.5*c;
}

vec3 spaceTwist( vec3 p, float angle )
{
    float c = cos(-angle*p.y);
    float s = sin(-angle*p.y);
    mat2  m = mat2(c,-s,s,c);
    return  vec3(m*p.xz,p.y);
}

float sdPlane( vec3 p )
{
   return p.y;
}

float sdOffset(vec3 p, float input, float factor)
{
        return input+factor ;
}

float sdSphere( vec3 p, vec3 center, float radius )
{
   return length(p-center)-radius;
}

float sdDifference(float a, float b)
{
	return max(-b,a);
}




float sdUnion( float d1, float d2 )
{
    return min(d1,d2);
}

vec4 minVec4( vec4 d1, vec4 d2 )
{
   return (d1.x<d2.x) ? d1 : d2;
}

//// This text will be replaced by the shape compiler before compiling the shader. 
vec4 map(in vec3 pos)
{
	REPLACE_WITH_SHAPE
}

vec4 castRay( in vec3 ro, in vec3 rd )
{
   float depth = MIN_DIST;
   vec3 m = vec3(-1.0, -1.0, -1.0);
   for( int i = 0; i < MAX_MARCHING_STEPS; i++ )
   {
       vec4 res = map( ro+rd*depth );
       if( res.x < EPSILON || depth > MAX_DIST ) 
       	break;
       depth += res.x*0.2;
       m = res.yzw;
   }
   return vec4( depth, m );
}

float softshadow( in vec3 ro, in vec3 rd, in float mint, in float tmax )
{
   float res = 1.0;
   float t = mint;
   for( int i=0; i<16; i++ )
   {
      float h = map( ro + rd*t ).x;
      res = min( res, 8.0*h/t );
      t += clamp( h, 0.02, 0.10 );
      if( h<0.001 || t>tmax ) break;
   }
   return clamp( res, 0.0, 1.0 );
}

vec3 estimateNormal(vec3 p) {
   return normalize(vec3(
       map(vec3(p.x + EPSILON, p.y, p.z)).x - map(vec3(p.x - EPSILON, p.y, p.z)).x,
       map(vec3(p.x, p.y + EPSILON, p.z)).x - map(vec3(p.x, p.y - EPSILON, p.z)).x,
       map(vec3(p.x, p.y, p.z  + EPSILON)).x - map(vec3(p.x, p.y, p.z - EPSILON)).x
   ));
}

vec4 render( in vec3 ro, in vec3 rd )
{
   vec4 res = castRay(ro,rd);
   if (res.x > MAX_DIST) 
   	return vec4(1.0, 0.0, 1.0, res.x);
    	
   float dist = res.x;
   vec3 col = res.yzw;
   vec3 pos = ro + dist * rd;
   vec3 nor = estimateNormal( pos );
   vec3 ref = reflect( rd, nor );

   vec3  lig = normalize(lightPos);
   float amb = clamp( 0.5+0.5*nor.y, 0.0, 1.0 );
   float dif = clamp( dot( nor, lig ), 0.0, 1.0 );
   float dom = smoothstep( -0.1, 0.1, ref.y );
   float spe = pow(clamp( dot( ref, lig ), 0.0, 1.0 ),16.0);
   dif *= softshadow( pos, lightPos, 0.02, 2.5 );
   //dom *= softshadow( pos, ref, 0.02, 2.5 );

   vec3 lin = vec3(0.0);
   lin += 1.30*dif*vec3(1.00,0.80,0.55);
   lin += 2.00*spe*vec3(1.00,0.90,0.70)*dif;
   lin += 0.40*amb*vec3(0.40,0.60,1.00);
   lin += 0.50*dom*vec3(0.40,0.60,1.00);
   col = col*lin;

   return vec4( clamp(col,0.0,1.0), res.x );
}

vec4 getCircle(in vec2 evalPos, in vec2 center, in float radius, in vec4 color)
{
    float d = distance(evalPos, center) - radius ; 
    
    /// Center. 
    if( d < -radius+0.01 )
    {
    	//color.w = 1.0-(abs(d) / 0.02);
        return color ; 
    }
    
    if( d < 0.005 && d > -0.005 )
    {
    	//color.w = 1.0 - ( abs(d) / 0.01);
        return color ; 
    }
    if( d < 0.012 && d > -0.012 )
    {
        color = vec4(0,0,0,1.0);
    	color.w = 1.0-(abs(d) / 0.02);
        return color ; 
    }
        
    return vec4(1,1,1,0) ; 
}

float distanceToLine(in vec2 pos, in vec2 a, in vec2 b)
{
    return abs((b.y-a.y)*pos.x - (b.x-a.x)*pos.y + b.x*a.y - b.y*a.x) / distance(a,b) ;
}

vec4 getLine(in vec2 evalPos, in vec2 a, in vec2 b, in vec4 color)
{
    float d = distanceToLine(evalPos, a, b);
    if(d < 0.01 && evalPos.x > -1.0)
    {
    	//color.w = 
        return color;
    }
    return vec4(1.0,1.0,1.0,0.0);
}

vec3 cameraPos(mat4 a_modelView)
{
 // Get the 3 basis vector planes at the camera origin and transform them into model space.
  //  
  // NOTE: Planes have to be transformed by the inverse transpose of a matrix
  //       Nice reference here: http://www.opengl.org/discussion_boards/showthread.php/159564-Clever-way-to-transform-plane-by-matrix
  //
  //       So for a transform to model space we need to do:
  //            inverse(transpose(inverse(MV)))
  //       This equals : transpose(MV) - see Lemma 5 in http://mathrefresher.blogspot.com.au/2007/06/transpose-of-matrix.html
  //
  // As each plane is simply (1,0,0,0), (0,1,0,0), (0,0,1,0) we can pull the data directly from the transpose matrix.
  //  
  mat4 modelViewT = transpose(a_modelView);
 
  // Get plane normals 
  vec3 n1=modelViewT[0].xyz;
  vec3 n2=modelViewT[1].xyz;
  vec3 n3=modelViewT[2].xyz;
 
  // Get plane distances
  float d1=modelViewT[0].w;
  float d2=modelViewT[1].w;
  float d3=modelViewT[2].w;
 
  // Get the intersection of these 3 planes 
  // (uisng math from RealTime Collision Detection by Christer Ericson)
  vec3 n2n3 = cross(n2, n3);
  float denom = dot(n1, n2n3);
 
  vec3 top = (n2n3 * d1) + cross(n1, (d3*n2) - (d2*n3));
  return top / -denom;
}


float depthSample(float linearDepth)
{
    float nonLinearDepth = (zFar + zNear - 2.0 * zNear * zFar / linearDepth) / (zFar - zNear);
    nonLinearDepth = (nonLinearDepth + 1.0) / 2.0;
    return nonLinearDepth;
}


void main()
{		
   /// Generate the ray 
   vec4 rayNormalized = vec4(gl_TexCoord[0].st, -1.0, 1.0) ;
   vec4 rayEye = inverse(gl_ProjectionMatrix) * rayNormalized ;   
   rayEye = vec4(rayEye.xy, -1.0, 0.0) ; 
   vec3 rayWorld = normalize((inverse(gl_ModelViewMatrix) * rayEye).xyz) ;

   vec3 rayOrigin=cameraPos(gl_ModelViewMatrix);      //set ray origin
   vec4 res = render( rayOrigin, rayWorld );
   gl_FragDepth = depthSample(res.w);
   gl_FragColor = vec4(res.xyz , 1.0 );   
   //gl_FragColor = vec4(res.w*vec3(0.1) , 1.0 );   
}

