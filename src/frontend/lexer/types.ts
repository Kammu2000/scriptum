export enum TokenType {
  NUMBER = "NUMBER", 
  LET = "LET",
  IDENTIFIER = "IDENTIFIER",
  OPEN_PARENTHESIS = "OPEN_PARENTHESIS",
  CLOSED_PARENTHESIS = "CLOSED_PARENTHESIS",
  BINARY_OPERATOR = "BINARY_OPERATOR",
  EQUALS = "EQUALS",
  EOF = "EOF",
} 

export interface Token {
  type: TokenType;
  value: string;
}

export const reserveWords = new Map<string, TokenType>([["let", TokenType.LET]]);

