module Hw2 where

data ASTResult = ASTError String | ASTJust (String, String, Int) deriving (Show, Read)
data ASTDatum = ASTSimpleDatum String | ASTLetDatum String deriving (Show, Read)
data AST = EmptyAST | ASTNode ASTDatum AST AST deriving (Show, Read)

isNumber :: String -> Bool
eagerEvaluation :: AST -> ASTResult
normalEvaluation :: AST -> ASTResult
-- DO NOT MODIFY OR DELETE THE LINES ABOVE -- 
-- IMPLEMENT isNumber, eagerEvaluation and normalEvaluation FUNCTIONS ACCORDING TO GIVEN SIGNATURES -- 

numChars = ["0","1","2","3","4","5","6","7","8","9"]

getDatum :: AST -> ASTDatum
getDatum (ASTNode a _ _) = a

getValue :: AST -> String
getValue EmptyAST = ""
getValue (ASTNode (ASTSimpleDatum a) _ _) = a
getValue (ASTNode (ASTLetDatum a) _ _) = a

getLeft :: AST -> AST
getLeft (ASTNode _ l _) = l

getRight :: AST -> AST
getRight (ASTNode _ _ r) = r


opr1 = ["plus", "times"]
opr2 = ["negate"]
opr3 = ["cat"]
opr4 = ["len"]
opr = opr1 ++ opr2 ++ opr3 ++ opr4 

typer = ["num", "str"]

inList :: String -> [String] -> Bool 
inList a [] = False
inList a (x:list) = if a == x then True
                    else inList a list


numCheck :: String -> Bool
numCheck [] = True 
numCheck (x:ls) = 
    if (not(inList [x] numChars))
        then False
    else 
        numCheck ls

negCheck :: String -> Bool
negCheck [] = True
negCheck (x:ls)
        | ((inList [x] numChars) == True) = True
        | x == '-' = True
        | otherwise = False
    
isNumber "" = False
isNumber "-" = False
isNumber (a:ls) = 
        if (((negCheck (a:ls)) == True) && ((numCheck ls) == True))
            then True
        else
            False

isLet :: AST -> Bool
isLet x = 
    not(inList (getValue x) typer) && not(inList (getValue x) opr) && not((getValue (getLeft x)) == "") && not((getValue (getRight x)) == "") 

isVar :: AST -> Bool
isVar x = 
    not(inList (getValue x) typer) && not(inList (getValue x) opr) && (getValue (getLeft x) == "") && (getValue (getRight x) == "")

numChecker :: String -> String
numChecker x =
    if (isNumber x)
        then "True"
    else
        "the value '" ++ x ++ "' is not a number!"

strChecker :: String -> String
strChecker x =
    if not(isNumber x)
        then "True"
    else
        "the value '" ++ x ++ "' is not a string!"

oprChecker :: String -> String -> String -> String
oprChecker x y z
    | (inList x opr1) && ((y /= "num") || (z /= "num"))
        = x ++ " operation is not defined between " ++ y ++ " and " ++ z ++ "!"
    |  (inList x opr2) && (y /= "num")
        = x ++ " operation is not defined on " ++ y ++ "!"
    |  (inList x opr3) && ((y /= "str") || (z /= "str"))
        = x ++ " operation is not defined between " ++ y ++ " and " ++ z ++ "!"
    |  (inList x opr4) && (y /= "str")
        = x ++ " operation is not defined on " ++ y ++ "!"
    | otherwise = "True"

normalBinder :: AST -> AST -> String -> AST
normalBinder EmptyAST _ _ = EmptyAST
normalBinder a x s 
    | (isLet a) && (getValue a == s) = (ASTNode (getDatum a) (normalBinder (getLeft a)(getLeft a)(getValue a))(normalBinder (getRight a)(getLeft a)(getValue a)))
    | (isVar a) && (getValue a == s) = x
    | (getValue a == "") = EmptyAST
    | otherwise = (ASTNode (getDatum a) (normalBinder (getLeft a) x s)(normalBinder (getRight a) x s ))

eagerBinder :: AST -> AST -> String -> AST
eagerBinder EmptyAST _ _ = EmptyAST
eagerBinder a x s
    | (isLet a) && (getValue a == s) = (ASTNode (getDatum a) (eagerBinder (getLeft a)(calculate(getLeft a))(getValue a))(eagerBinder (getRight a)(calculate(getLeft a))(getValue a)))
    | (isVar a) && (getValue a == s) = x
    | (getValue a == "") = EmptyAST
    | otherwise = (ASTNode (getDatum a) (eagerBinder (getLeft a) x s)(eagerBinder (getRight a) x s ))


traveler :: AST -> String -> [String] -> AST
traveler a "normal" lst
    | (getValue a == "") = EmptyAST
    | ((isLet a) && not(inList (getValue a) lst)) = traveler (getRight (normalBinder a (getLeft a)(getValue a))) "normal" ((getValue a):lst)
    | ((isLet a) && (inList (getValue a) lst)) = traveler (getRight a) "normal" lst
    | otherwise = (ASTNode (getDatum a)(traveler (getLeft a) "normal" lst)(traveler (getRight a) "normal" lst))

traveler a "eager" lst
    | (getValue a == "") = EmptyAST
    | ((isLet a) && not(inList (getValue a) lst)) = traveler (getRight (eagerBinder a (calculate(getLeft a))(getValue a))) "eager" ((getValue a):lst)
    | ((isLet a) && (inList (getValue a) lst)) = traveler (getRight a) "eager" lst
    | otherwise = (ASTNode (getDatum a)(traveler (getLeft a) "eager" lst)(traveler (getRight a) "eager" lst))

--turnString((turnInt (getValue(getLeft x))) + (turnInt (getValue(getLeft y))))

--(ASTNode (ASTSimpleDatum "error") (ASTNode (ASTSimpleDatum (oprChecker "plus" (getValue x) (getValue y)))(EmptyAST)(EmptyAST))(EmptyAST)) 

--(numChecker (getValue x))
add :: AST -> AST -> AST
add x y
    | (isLet (x)) || (isLet(y)) = add (calculate(traveler x "eager" [])) (calculate(traveler y "eager" []))
    | (getValue x) == "error" = x
    | (getValue y) == "error" = y
    | ((oprChecker ("plus") (getValue x) (getValue y)) /= "True") = (ASTNode (ASTSimpleDatum "error") (ASTNode (ASTSimpleDatum (oprChecker "plus" (getValue x) (getValue y)))(EmptyAST)(EmptyAST))(EmptyAST)) 
    | (numChecker (getValue (getLeft x)) /= "True") = (ASTNode (ASTSimpleDatum "error") (ASTNode (ASTSimpleDatum (numChecker (getValue (getLeft x))))(EmptyAST)(EmptyAST))(EmptyAST)) 
    | (numChecker (getValue (getLeft y)) /= "True") = (ASTNode (ASTSimpleDatum "error") (ASTNode (ASTSimpleDatum (numChecker (getValue (getLeft y))))(EmptyAST)(EmptyAST))(EmptyAST)) 
    | otherwise = (ASTNode (ASTSimpleDatum "num") (ASTNode (ASTSimpleDatum (turnString((turnInt (getValue(getLeft x))) + (turnInt (getValue(getLeft y))))))(EmptyAST)(EmptyAST))(EmptyAST)) 

mult :: AST -> AST -> AST
mult x y
    | (isLet (x)) || (isLet(y)) = mult (calculate(traveler x "eager" [])) (calculate(traveler y "eager" []))
    | (getValue x) == "error" = x
    | (getValue y) == "error" = y
    | ((oprChecker ("times") (getValue x) (getValue y)) /= "True") = (ASTNode (ASTSimpleDatum "error") (ASTNode (ASTSimpleDatum (oprChecker "times" (getValue x) (getValue y)))(EmptyAST)(EmptyAST))(EmptyAST)) 
    | (numChecker (getValue (getLeft x)) /= "True") = (ASTNode (ASTSimpleDatum "error") (ASTNode (ASTSimpleDatum (numChecker (getValue (getLeft x))))(EmptyAST)(EmptyAST))(EmptyAST)) 
    | (numChecker (getValue (getLeft y)) /= "True") = (ASTNode (ASTSimpleDatum "error") (ASTNode (ASTSimpleDatum (numChecker (getValue (getLeft y))))(EmptyAST)(EmptyAST))(EmptyAST)) 
    | otherwise = (ASTNode (ASTSimpleDatum "num") (ASTNode (ASTSimpleDatum (turnString((turnInt (getValue(getLeft x))) * (turnInt (getValue(getLeft y))))))(EmptyAST)(EmptyAST))(EmptyAST)) 

neg :: AST -> AST
neg x
    | (isLet (x)) = neg (calculate(traveler x "eager" []))
    | (getValue x) == "error" = x
    | ((oprChecker ("negate") (getValue x) "") /= "True") = (ASTNode (ASTSimpleDatum "error") (ASTNode (ASTSimpleDatum (oprChecker "negate" (getValue x) ""))(EmptyAST)(EmptyAST))(EmptyAST)) 
    | (numChecker (getValue (getLeft x)) /= "True") = (ASTNode (ASTSimpleDatum "error") (ASTNode (ASTSimpleDatum (numChecker (getValue (getLeft x))))(EmptyAST)(EmptyAST))(EmptyAST)) 
    | otherwise = (ASTNode (ASTSimpleDatum "num") (ASTNode (ASTSimpleDatum (turnString ( - (turnInt (getValue(getLeft x))))))(EmptyAST)(EmptyAST))(EmptyAST)) 


cat :: AST -> AST -> AST
cat x y
    | (isLet (x)) || (isLet(y)) = cat (calculate(traveler x "eager" [])) (calculate(traveler y "eager" []))
    | (isLet x) = traveler x "eager" []
    | (isLet y) = traveler y "eager" []
    | (getValue x) == "error" = x
    | (getValue y) == "error" = y
    | ((oprChecker ("cat") (getValue x) (getValue y)) /= "True") = (ASTNode (ASTSimpleDatum "error") (ASTNode (ASTSimpleDatum (oprChecker "cat" (getValue x) (getValue y)))(EmptyAST)(EmptyAST))(EmptyAST)) 
    -- | (strChecker (getValue (getLeft x)) /= "True") = (ASTNode (ASTSimpleDatum "error") (ASTNode (ASTSimpleDatum (strChecker (getValue (getLeft x))))(EmptyAST)(EmptyAST))(EmptyAST)) 
    -- | (strChecker (getValue (getLeft y)) /= "True") = (ASTNode (ASTSimpleDatum "error") (ASTNode (ASTSimpleDatum (strChecker (getValue (getLeft y))))(EmptyAST)(EmptyAST))(EmptyAST)) 
    | otherwise = (ASTNode (ASTSimpleDatum "str") (ASTNode (ASTSimpleDatum ((getValue(getLeft x)) ++ (getValue(getLeft y))))(EmptyAST)(EmptyAST))(EmptyAST))

len :: AST -> AST
len x
    | (isLet (x)) = len (calculate(traveler x "eager" []))
    | (getValue x) == "error" = x
    | ((oprChecker ("len") (getValue x) "") /= "True") = (ASTNode (ASTSimpleDatum "error") (ASTNode (ASTSimpleDatum (oprChecker "len" (getValue x) ""))(EmptyAST)(EmptyAST))(EmptyAST)) 
    -- | (strChecker (getValue (getLeft x)) /= "True") = (ASTNode (ASTSimpleDatum "error") (ASTNode (ASTSimpleDatum (strChecker (getValue (getLeft x))))(EmptyAST)(EmptyAST))(EmptyAST)) 
    | otherwise = (ASTNode (ASTSimpleDatum "num") (ASTNode (ASTSimpleDatum (turnString(length (getValue(getLeft x)))))(EmptyAST)(EmptyAST))(EmptyAST))

turnInt :: String -> Int
turnInt a = read a::Int

turnString :: Int -> String
turnString a = show a

calculate :: AST -> AST
calculate x
    | (getValue (getLeft x)) == "error" = getLeft x
    | (getValue (getRight x)) == "error" = getRight x
    | (getValue x) == "plus" = add (calculate (getLeft x))(calculate (getRight x))
    | (getValue x) == "times" = mult (calculate (getLeft x))(calculate (getRight x))
    | (getValue x) == "negate" = neg (calculate (getLeft x))
    | (getValue x) == "cat" = cat (calculate (getLeft x)) (calculate (getRight x))
    | (getValue x) == "len" = len (calculate (getLeft x))
    | ((getValue x) == "num") && (numChecker (getValue (getLeft x)) /= "True") = (ASTNode (ASTSimpleDatum "error") (ASTNode (ASTSimpleDatum (numChecker (getValue (getLeft x))))(EmptyAST)(EmptyAST))(EmptyAST))
    | otherwise = x


--normalOprCounter :: AST -> Int -> Int
normalOprCounter EmptyAST = 0
normalOprCounter x
    | (inList (getValue x) opr) = 1 + normalOprCounter (getLeft x) + normalOprCounter (getRight x)
    | otherwise = 0 + normalOprCounter (getLeft x) + normalOprCounter (getRight x)

--eagerOprCounter :: AST -> Int -> Int
eagerOprCounter EmptyAST = 0
eagerOprCounter x
    | (getValue x) == "" = 0
    | (isLet x) = normalOprCounter (getLeft x) + eagerOprCounter (getRight x)
    | otherwise = 0 + eagerOprCounter (getLeft x) + eagerOprCounter (getRight x)
    

normalEvaluation x
    | getValue(calculate a) == "error" = ASTError (getValue(getLeft(calculate a)))
    | otherwise = ASTJust(getValue(getLeft(calculate a)), getValue(calculate a), normalOprCounter a)
    where a = (traveler x "normal" [])
 
eagerEvaluation x
    | getValue(calculate a) == "error" = ASTError (getValue(getLeft(calculate a)))
    | otherwise = ASTJust(getValue(getLeft(calculate a)), getValue(calculate a), normalOprCounter a + eagerOprCounter x)
    where a = (traveler x "eager" [])