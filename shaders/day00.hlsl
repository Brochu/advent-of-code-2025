RWStructuredBuffer<uint> Output : register(u0);

[numthreads(16, 1, 1)] // 16 threads per group
void main(uint DTid : SV_GroupIndex)
{
    Output[DTid] = 70;
}
