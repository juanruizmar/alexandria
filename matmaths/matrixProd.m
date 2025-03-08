% It computes the element-by-element matrix-product of two sets of matrix (same dimensions and same size for sets)

function C = matrixProd(A,B)
    C = sum(repmat(A,[1,1,1,size(B,2)]).*permute(repmat(B,[1,1,1,size(A,1)]),[4,1,3,2]),2);
    C = permute(squeeze(C(:,1,:,:)), [1,3,2]);