__kernel void evalField(global const float4* A, global float* C)
{
     C[get_global_id(0)]=A[get_global_id(0)].x;
}
