RWStructuredBuffer<uint> Output : register(u0);

[numthreads(8, 1, 1)] // 8 threads per group
void main(uint3 DTid : SV_DispatchThreadID)
{
    Output[DTid.x] = DTid.x;
}
