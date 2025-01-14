% This function computes coefficients that fit better between x and y data, by a polynomial of a specific degree
% It returns the coefficients of this polynomial, and it determinism coefficient (R^2)

% NEVER TESTED

function [coefs, R_sqr] = fit_polynomial(x, y, degree)
    fprintf("PAY ATTENTION! function [coefs, R_sqr] = fit_polynomial(x, y, degree) is now being used by the first time");

    coefs=polyfit(x,y,degree);
    R_sqr = 1 - (sum((y - polyval(p, x)).^2) / sum((y - mean(y)).^2));
