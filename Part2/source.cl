__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST; 

__kernel void bw(read_only image2d_t in, write_only image2d_t out)
{
	int2 coord = (int2)(get_global_id(0), get_global_id(1)); 
	float4 pixel = read_imagef(in, sampler, coord);
	pixel.xyz = pixel.x*0.299 + pixel.y*0.587 + pixel.z*0.114;
	write_imagef(out, coord, pixel);
}

__kernel void reduction_vector(__global float4* data, 
      __local float4* partial_sums) {

   int lid = get_local_id(0);
   int group_size = get_local_size(0);

   partial_sums[lid] = data[get_global_id(0)];
   barrier(CLK_LOCAL_MEM_FENCE);

   for(int i = group_size/2; i>0; i >>= 1) {
      if(lid < i) {
         partial_sums[lid] += partial_sums[lid + i];
      }
      barrier(CLK_LOCAL_MEM_FENCE);
   }

   if(lid == 0) {
      data[get_group_id(0)] = partial_sums[0];
   }
}

__kernel void reduction_complete(__global float4* data, 
      __local float4* partial_sums, __global float* sum) {

   int lid = get_local_id(0);
   int group_size = get_local_size(0);

   partial_sums[lid] = data[get_local_id(0)];
   barrier(CLK_LOCAL_MEM_FENCE);

   for(int i = group_size/2; i>0; i >>= 1) {
      if(lid < i) {
         partial_sums[lid] += partial_sums[lid + i];
      }
      barrier(CLK_LOCAL_MEM_FENCE);
   }

   if(lid == 0) {
      *sum = partial_sums[0].s0 + partial_sums[0].s1 +
             partial_sums[0].s2 + partial_sums[0].s3;
   }
}
