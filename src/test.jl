using LinearAlgebra
ncol = 2
A = [1 2]
B = [1 4 ; 1 4]
C = [0 0 ; 0 0]
print(A*B,'\n')

# function row(i, C, A, B,ncol)
#     for k = 1:ncol
#         C[i,:] += A[i,k] * B[k,:]
#     end
# end

# row(2,C,A,B,ncol)
# print(A * B,'\n',C)