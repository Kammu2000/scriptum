// imports 
import { Token, TokenType } from "../lexer/types";

export default class ParserContext {
  public pos: number;
  public tokens: Token[];

  constructor(tokens: Token[], pos: number){
    this.tokens = tokens;
    this.pos = pos;
  }

  peek(): Token {
    return this.tokens[this.pos];
  }

  next(): Token {
    return this.tokens[this.pos + 1];
  }

  eat(): Token {
    return this.tokens[this.pos++];
  }

  endOf(): boolean {
    return this.peek().type === TokenType.EOF;
  }
}  

