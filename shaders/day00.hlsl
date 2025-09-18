RWStructuredBuffer<uint> Output : register(u0, space1);

[numthreads(16, 1, 1)] // 16 threads per group
void main(uint DTid : SV_GroupIndex)
{
    Output[DTid] = 70;
}
