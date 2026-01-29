export enum TokenType {
  NUMBER = "NUMBER", 
  IDENTIFIER = "IDENTIFIER",

  // parenthesis 
  OPEN_PARENTHESIS = "OPEN_PARENTHESIS",
  CLOSED_PARENTHESIS = "CLOSED_PARENTHESIS",

  // braces
  OPEN_BRACES = "OPEN_BRACES",
  CLOSED_BRACES = "CLOSED_BRACES",

  // operators
  BINARY_OPERATOR = "BINARY_OPERATOR",
  EQUALS = "EQUALS",

  // RESERVED_WORDS
  LET = "LET",
  IF = "IF",
  ELSE = "ELSE",
  FOR = "FOR",
  WHILE = "WHILE",

  // end of file 
  EOF = "EOF",
} 

export interface Token {
  type: TokenType;
  value: string;
}

export const RESERVED_WORDS = new Map<string, TokenType>([["let", TokenType.LET], ["if", TokenType.IF], ["else", TokenType.ELSE], ["for", TokenType.FOR], ["while", TokenType.WHILE]]);

