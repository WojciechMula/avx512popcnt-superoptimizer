62 f1 fd 48 7f 29    	vmovdqa64 %zmm5,(%rcx)
f3 4c 0f b8 01       	popcnt (%rcx),%r8
f3 4c 0f b8 49 08    	popcnt 0x8(%rcx),%r9
f3 4c 0f b8 51 10    	popcnt 0x10(%rcx),%r10
f3 4c 0f b8 59 18    	popcnt 0x18(%rcx),%r11
f3 4c 0f b8 61 20    	popcnt 0x20(%rcx),%r12
f3 4c 0f b8 69 28    	popcnt 0x28(%rcx),%r13
f3 4c 0f b8 71 30    	popcnt 0x30(%rcx),%r14
f3 4c 0f b8 79 38    	popcnt 0x38(%rcx),%r15
48 31 ff             	xor    %rdi,%rdi
4c 01 c7             	add    %r8,%rdi
4c 01 cf             	add    %r9,%rdi
4c 01 d7             	add    %r10,%rdi
4c 01 df             	add    %r11,%rdi
4c 01 e7             	add    %r12,%rdi
4c 01 ef             	add    %r13,%rdi
4c 01 f7             	add    %r14,%rdi
4c 01 ff             	add    %r15,%rdi
62 61 fd 48 6f 30    	vmovdqa64 (%rax),%zmm30
62 61 fd 48 6f 78 01 	vmovdqa64 0x40(%rax),%zmm31
62 71 fd 48 6f d0    	vmovdqa64 %zmm0,%zmm10
62 93 05 40 25 c6 96 	vpternlogd $0x96,%zmm30,%zmm31,%zmm0
62 13 05 40 25 d6 e8 	vpternlogd $0xe8,%zmm30,%zmm31,%zmm10
62 61 fd 48 6f 70 02 	vmovdqa64 0x80(%rax),%zmm30
62 61 fd 48 6f 78 03 	vmovdqa64 0xc0(%rax),%zmm31
62 71 fd 48 6f d8    	vmovdqa64 %zmm0,%zmm11
62 93 05 40 25 c6 96 	vpternlogd $0x96,%zmm30,%zmm31,%zmm0
62 13 05 40 25 de e8 	vpternlogd $0xe8,%zmm30,%zmm31,%zmm11
62 61 fd 48 6f 70 04 	vmovdqa64 0x100(%rax),%zmm30
62 61 fd 48 6f 78 05 	vmovdqa64 0x140(%rax),%zmm31
62 71 fd 48 6f e0    	vmovdqa64 %zmm0,%zmm12
62 93 05 40 25 c6 96 	vpternlogd $0x96,%zmm30,%zmm31,%zmm0
62 13 05 40 25 e6 e8 	vpternlogd $0xe8,%zmm30,%zmm31,%zmm12
62 61 fd 48 6f 70 06 	vmovdqa64 0x180(%rax),%zmm30
62 61 fd 48 6f 78 07 	vmovdqa64 0x1c0(%rax),%zmm31
62 71 fd 48 6f e8    	vmovdqa64 %zmm0,%zmm13
62 93 05 40 25 c6 96 	vpternlogd $0x96,%zmm30,%zmm31,%zmm0
62 13 05 40 25 ee e8 	vpternlogd $0xe8,%zmm30,%zmm31,%zmm13
62 61 fd 48 6f 70 08 	vmovdqa64 0x200(%rax),%zmm30
62 61 fd 48 6f 78 09 	vmovdqa64 0x240(%rax),%zmm31
62 71 fd 48 6f f0    	vmovdqa64 %zmm0,%zmm14
62 93 05 40 25 c6 96 	vpternlogd $0x96,%zmm30,%zmm31,%zmm0
62 13 05 40 25 f6 e8 	vpternlogd $0xe8,%zmm30,%zmm31,%zmm14
62 61 fd 48 6f 70 0a 	vmovdqa64 0x280(%rax),%zmm30
62 61 fd 48 6f 78 0b 	vmovdqa64 0x2c0(%rax),%zmm31
62 71 fd 48 6f f8    	vmovdqa64 %zmm0,%zmm15
62 93 05 40 25 c6 96 	vpternlogd $0x96,%zmm30,%zmm31,%zmm0
62 13 05 40 25 fe e8 	vpternlogd $0xe8,%zmm30,%zmm31,%zmm15
62 61 fd 48 6f 70 0c 	vmovdqa64 0x300(%rax),%zmm30
62 61 fd 48 6f 78 0d 	vmovdqa64 0x340(%rax),%zmm31
62 e1 fd 48 6f c0    	vmovdqa64 %zmm0,%zmm16
62 93 05 40 25 c6 96 	vpternlogd $0x96,%zmm30,%zmm31,%zmm0
62 83 05 40 25 c6 e8 	vpternlogd $0xe8,%zmm30,%zmm31,%zmm16
62 61 fd 48 6f 70 0e 	vmovdqa64 0x380(%rax),%zmm30
62 61 fd 48 6f 78 0f 	vmovdqa64 0x3c0(%rax),%zmm31
62 e1 fd 48 6f c8    	vmovdqa64 %zmm0,%zmm17
62 93 05 40 25 c6 96 	vpternlogd $0x96,%zmm30,%zmm31,%zmm0
62 83 05 40 25 ce e8 	vpternlogd $0xe8,%zmm30,%zmm31,%zmm17
62 61 fd 48 6f 70 10 	vmovdqa64 0x400(%rax),%zmm30
62 61 fd 48 6f 78 11 	vmovdqa64 0x440(%rax),%zmm31
62 e1 fd 48 6f d0    	vmovdqa64 %zmm0,%zmm18
62 93 05 40 25 c6 96 	vpternlogd $0x96,%zmm30,%zmm31,%zmm0
62 83 05 40 25 d6 e8 	vpternlogd $0xe8,%zmm30,%zmm31,%zmm18
62 61 fd 48 6f 70 12 	vmovdqa64 0x480(%rax),%zmm30
62 61 fd 48 6f 78 13 	vmovdqa64 0x4c0(%rax),%zmm31
62 e1 fd 48 6f d8    	vmovdqa64 %zmm0,%zmm19
62 93 05 40 25 c6 96 	vpternlogd $0x96,%zmm30,%zmm31,%zmm0
62 83 05 40 25 de e8 	vpternlogd $0xe8,%zmm30,%zmm31,%zmm19
62 61 fd 48 6f 70 14 	vmovdqa64 0x500(%rax),%zmm30
62 61 fd 48 6f 78 15 	vmovdqa64 0x540(%rax),%zmm31
62 e1 fd 48 6f e0    	vmovdqa64 %zmm0,%zmm20
62 93 05 40 25 c6 96 	vpternlogd $0x96,%zmm30,%zmm31,%zmm0
62 83 05 40 25 e6 e8 	vpternlogd $0xe8,%zmm30,%zmm31,%zmm20
62 61 fd 48 6f 70 16 	vmovdqa64 0x580(%rax),%zmm30
62 61 fd 48 6f 78 17 	vmovdqa64 0x5c0(%rax),%zmm31
62 e1 fd 48 6f e8    	vmovdqa64 %zmm0,%zmm21
62 93 05 40 25 c6 96 	vpternlogd $0x96,%zmm30,%zmm31,%zmm0
62 83 05 40 25 ee e8 	vpternlogd $0xe8,%zmm30,%zmm31,%zmm21
62 61 fd 48 6f 70 18 	vmovdqa64 0x600(%rax),%zmm30
62 61 fd 48 6f 78 19 	vmovdqa64 0x640(%rax),%zmm31
62 e1 fd 48 6f f0    	vmovdqa64 %zmm0,%zmm22
62 93 05 40 25 c6 96 	vpternlogd $0x96,%zmm30,%zmm31,%zmm0
62 83 05 40 25 f6 e8 	vpternlogd $0xe8,%zmm30,%zmm31,%zmm22
62 61 fd 48 6f 70 1a 	vmovdqa64 0x680(%rax),%zmm30
62 61 fd 48 6f 78 1b 	vmovdqa64 0x6c0(%rax),%zmm31
62 e1 fd 48 6f f8    	vmovdqa64 %zmm0,%zmm23
62 93 05 40 25 c6 96 	vpternlogd $0x96,%zmm30,%zmm31,%zmm0
62 83 05 40 25 fe e8 	vpternlogd $0xe8,%zmm30,%zmm31,%zmm23
62 61 fd 48 6f 70 1c 	vmovdqa64 0x700(%rax),%zmm30
62 61 fd 48 6f 78 1d 	vmovdqa64 0x740(%rax),%zmm31
62 61 fd 48 6f c0    	vmovdqa64 %zmm0,%zmm24
62 93 05 40 25 c6 96 	vpternlogd $0x96,%zmm30,%zmm31,%zmm0
62 03 05 40 25 c6 e8 	vpternlogd $0xe8,%zmm30,%zmm31,%zmm24
62 61 fd 48 6f 70 1e 	vmovdqa64 0x780(%rax),%zmm30
62 61 fd 48 6f 78 1f 	vmovdqa64 0x7c0(%rax),%zmm31
62 61 fd 48 6f c8    	vmovdqa64 %zmm0,%zmm25
62 93 05 40 25 c6 96 	vpternlogd $0x96,%zmm30,%zmm31,%zmm0
62 03 05 40 25 ce e8 	vpternlogd $0xe8,%zmm30,%zmm31,%zmm25
62 61 fd 48 6f f1    	vmovdqa64 %zmm1,%zmm30
62 d3 25 48 25 ca 96 	vpternlogd $0x96,%zmm10,%zmm11,%zmm1
62 13 25 48 25 d6 e8 	vpternlogd $0xe8,%zmm30,%zmm11,%zmm10
62 61 fd 48 6f f1    	vmovdqa64 %zmm1,%zmm30
62 d3 15 48 25 cc 96 	vpternlogd $0x96,%zmm12,%zmm13,%zmm1
62 13 15 48 25 e6 e8 	vpternlogd $0xe8,%zmm30,%zmm13,%zmm12
62 61 fd 48 6f f1    	vmovdqa64 %zmm1,%zmm30
62 d3 05 48 25 ce 96 	vpternlogd $0x96,%zmm14,%zmm15,%zmm1
62 13 05 48 25 f6 e8 	vpternlogd $0xe8,%zmm30,%zmm15,%zmm14
62 61 fd 48 6f f1    	vmovdqa64 %zmm1,%zmm30
62 b3 75 40 25 c8 96 	vpternlogd $0x96,%zmm16,%zmm17,%zmm1
62 83 75 40 25 c6 e8 	vpternlogd $0xe8,%zmm30,%zmm17,%zmm16
62 61 fd 48 6f f1    	vmovdqa64 %zmm1,%zmm30
62 b3 65 40 25 ca 96 	vpternlogd $0x96,%zmm18,%zmm19,%zmm1
62 83 65 40 25 d6 e8 	vpternlogd $0xe8,%zmm30,%zmm19,%zmm18
62 61 fd 48 6f f1    	vmovdqa64 %zmm1,%zmm30
62 b3 55 40 25 cc 96 	vpternlogd $0x96,%zmm20,%zmm21,%zmm1
62 83 55 40 25 e6 e8 	vpternlogd $0xe8,%zmm30,%zmm21,%zmm20
62 61 fd 48 6f f1    	vmovdqa64 %zmm1,%zmm30
62 b3 45 40 25 ce 96 	vpternlogd $0x96,%zmm22,%zmm23,%zmm1
62 83 45 40 25 f6 e8 	vpternlogd $0xe8,%zmm30,%zmm23,%zmm22
62 61 fd 48 6f f1    	vmovdqa64 %zmm1,%zmm30
62 93 35 40 25 c8 96 	vpternlogd $0x96,%zmm24,%zmm25,%zmm1
62 03 35 40 25 c6 e8 	vpternlogd $0xe8,%zmm30,%zmm25,%zmm24
62 61 fd 48 6f f2    	vmovdqa64 %zmm2,%zmm30
62 d3 1d 48 25 d2 96 	vpternlogd $0x96,%zmm10,%zmm12,%zmm2
62 13 1d 48 25 d6 e8 	vpternlogd $0xe8,%zmm30,%zmm12,%zmm10
62 61 fd 48 6f f2    	vmovdqa64 %zmm2,%zmm30
62 d3 7d 40 25 d6 96 	vpternlogd $0x96,%zmm14,%zmm16,%zmm2
62 13 7d 40 25 f6 e8 	vpternlogd $0xe8,%zmm30,%zmm16,%zmm14
62 61 fd 48 6f f2    	vmovdqa64 %zmm2,%zmm30
62 b3 5d 40 25 d2 96 	vpternlogd $0x96,%zmm18,%zmm20,%zmm2
62 83 5d 40 25 d6 e8 	vpternlogd $0xe8,%zmm30,%zmm20,%zmm18
62 61 fd 48 6f f2    	vmovdqa64 %zmm2,%zmm30
62 b3 3d 40 25 d6 96 	vpternlogd $0x96,%zmm22,%zmm24,%zmm2
62 83 3d 40 25 f6 e8 	vpternlogd $0xe8,%zmm30,%zmm24,%zmm22
62 61 fd 48 6f f3    	vmovdqa64 %zmm3,%zmm30
62 d3 0d 48 25 da 96 	vpternlogd $0x96,%zmm10,%zmm14,%zmm3
62 13 0d 48 25 d6 e8 	vpternlogd $0xe8,%zmm30,%zmm14,%zmm10
62 61 fd 48 6f f3    	vmovdqa64 %zmm3,%zmm30
62 b3 4d 40 25 da 96 	vpternlogd $0x96,%zmm18,%zmm22,%zmm3
62 83 4d 40 25 d6 e8 	vpternlogd $0xe8,%zmm30,%zmm22,%zmm18
62 f1 fd 48 6f ec    	vmovdqa64 %zmm4,%zmm5
62 d3 6d 40 25 e2 96 	vpternlogd $0x96,%zmm10,%zmm18,%zmm4
62 d3 6d 40 25 ea e8 	vpternlogd $0xe8,%zmm10,%zmm18,%zmm5
