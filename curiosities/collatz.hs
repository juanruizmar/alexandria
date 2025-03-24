-- Archivo: collatz.hs

import System.Environment (getArgs)

collatzSteps :: Integer -> Integer
collatzSteps n
    | n <= 0    = error "El número debe ser positivo"
    | n == 1    = 0
    | even n    = 1 + collatzSteps (n `div` 2)
    | otherwise = 1 + collatzSteps (3 * n + 1)

factorial :: Integer -> Integer
factorial n
    | n < 0     = error "El número debe ser no negativo"
    | n == 0    = 1
    | otherwise = n * factorial (n - 1)

main :: IO ()
main = do
    args <- getArgs
    case args of
        [input] -> do
            let number = read input :: Integer
            if number > 0
                then putStrLn $ "Cantidad de pasos: " ++ show (collatzSteps (factorial number))
                else putStrLn "Por favor, introduce un número positivo."
        _ -> putStrLn "Uso: ./collatz <número positivo>"
