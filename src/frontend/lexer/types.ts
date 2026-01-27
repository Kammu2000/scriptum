export enum TokenType {
  NUMBER = "NUMBER", 
  LET = "LET",
  IF = "IF",
  ELSE = "ELSE",
  IDENTIFIER = "IDENTIFIER",
  OPEN_PARENTHESIS = "OPEN_PARENTHESIS",
  CLOSED_PARENTHESIS = "CLOSED_PARENTHESIS",
  OPEN_BRACES = "OPEN_BRACES",
  CLOSE_BRACES = "CLOSE_BRACES",
  BINARY_OPERATOR = "BINARY_OPERATOR",
  EQUALS = "EQUALS",
  EOF = "EOF",
} 

export interface Token {
  type: TokenType;
  value: string;
}

export const reserveWords = new Map<string, TokenType>([["let", TokenType.LET], ["if", TokenType.IF], ["else", TokenType.ELSE]]);

