module Hw1 where

type Mapping = [(String, String, String)]
data AST = EmptyAST | ASTNode String AST AST deriving (Show, Read)

writeExpression :: (AST, Mapping) -> String
evaluateAST :: (AST, Mapping) -> (AST, String)
-- DO NOT MODIFY OR DELETE THE LINES ABOVE -- 
-- IMPLEMENT writeExpression and evaluateAST FUNCTION ACCORDING TO GIVEN SIGNATURES --

getValue :: AST -> String
getValue (ASTNode a _ _) = a

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

findOpr :: String->String
findOpr "plus" = "+"
findOpr "times" = "*"
findOpr "negate" = "-"
findOpr "cat" = "++"
findOpr "len" = "length "
findOpr _ = ""



grouper :: AST -> String
grouper (ASTNode a l r) = if inList a opr1 == True 
                            then "(" ++ grouper l ++ findOpr a ++ grouper r ++ ")"  --opr1
                          else if inList a opr2 == True
                                then "(" ++ findOpr a ++ grouper l ++ ")"  --opr2
                          else if inList a opr3 == True
                                 then "(" ++  grouper l ++ findOpr a ++ grouper r ++ ")" --opr3
                          else if inList a opr4 == True
                                 then "(" ++ findOpr a ++ grouper l ++ ")"  --opr4
                          else if a  == "str"
                                 then "\"" ++ grouper l ++ "\""
                          else if a == "num"
                                 then grouper l
                          else
                                 a

emptyChecker :: Mapping -> String
emptyChecker [] = ""
emptyChecker _ = ";"

varMaker :: Mapping -> String
varMaker [] = ""
varMaker ((x,y,z):list) = if y == "str"
                            then x ++ "=" ++ "\"" ++ z ++ "\"" ++ emptyChecker list ++ varMaker list
                          else                   
                            x ++ "=" ++  z ++ emptyChecker list ++ varMaker list

writeExpression (ast, map) = if map /= []
                                then "let " ++ varMaker map ++ " in " ++ grouper ast
                            else grouper ast

getMapName :: Mapping -> String
getMapName [] = ""
getMapName ((a,b,c):xs) = a

getMapType :: Mapping -> String
getMapType [] = ""
getMapType ((a,b,c):xs) = b

getMapValue :: Mapping -> String
getMapValue [] = ""
getMapValue ((a,b,c):xs) = c

matchMapName :: Mapping -> String -> Mapping
matchMapName [] a = []
matchMapName (x:xs) a = if getMapName (x:xs) == a
                            then (x:xs)
                        else
                            matchMapName (xs) a

former :: AST -> Mapping -> AST
former (ASTNode a l r) map = if (inList a opr1 == True) || (inList a opr3 == True)                      --checking binary operators
                                    then ASTNode a (former l map)(former r map)
                            else if (inList a opr2 == True) || (inList a opr4 == True)               --checking unary operator
                                    then ASTNode a (former l map)(EmptyAST)
                            else if inList a typer == True                                       --checking type
                                    then ASTNode a l r
                            else                                                            --checking variables
                                    let ls = matchMapName map a 
                                    in ASTNode (getMapType ls) (ASTNode (getMapValue ls) EmptyAST EmptyAST)(EmptyAST)

add :: Int -> Int -> Int
add x y = x+y

mult :: Int -> Int -> Int
mult x y = x*y

neg :: Int -> Int
neg x = -x

cat :: String -> String -> String
cat x y = x ++ y

len :: String -> Int
len a = length a

turnInt :: String -> Int
turnInt a = read a::Int

turnString :: Int -> String
turnString a = show a

calculate :: AST -> String
calculate (ASTNode a l r) = if (a == "plus")
                                then turnString (add (turnInt (calculate l))(turnInt (calculate r)))
                            else if (a == "times")
                                then turnString (mult (turnInt (calculate l))(turnInt (calculate r)))
                            else if (a == "negate")
                                then turnString (negate (turnInt (calculate l)))
                            else if (a == "cat")
                                then cat (calculate l)(calculate r)
                            else if (a == "len")
                                then turnString (len (calculate l))
                            else
                                getValue l 

evaluateAST(ast, map) = ((former ast map), calculate (former ast map))