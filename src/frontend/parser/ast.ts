// imports 
import { tokenize } from "../lexer/tokenize";
import { Token, TokenType } from "../lexer/types";
import { Expression, Program, Statement, ProgramKind, ExpressionKind, StatementKind } from "./types";

export default class Parser {

  private pos: number;
  private tokens: Token[];

  constructor(){
    this.tokens = [];
    this.pos = 0;
  }

  peek(): Token {
    return this.tokens[this.pos];
  }

  eat(): Token {
    return this.tokens[this.pos++];
  }

  endOf(): boolean {
    return this.peek().type === TokenType.EOF;
  }
  
  buildAST(sourceCode: string): Program {
    const program: Program = {
      kind: ProgramKind.Program,
      body: [],
    };
    
    this.tokens = tokenize(sourceCode); 
    
    while(!this.endOf()){
      const stmt = this.parseStatement();
      program.body.push(stmt);
    }

    return program;
  }

  private parseStatement(): Statement {
    const expr = this.parseExpression();
    return { kind: StatementKind.ExpressionStatement, expression: expr };
  }
  
  private parseExpression(): Expression {
    
    return this.parseAdditiveExpression();
  }

  private parseAdditiveExpression(): Expression {
    let left = this.parseMultiplicativeExpression();

    while(this.peek().value === "+" || this.peek().value === "-"){
      const op = this.eat().value;
      const right = this.parseMultiplicativeExpression();

      left = {
        kind: ExpressionKind.BinayExpression,
        left,
        right, 
        op,
      }
    }

    return left;
  }


  private parseMultiplicativeExpression(): Expression {
    
    let left = this.parsePrimaryExpression();

    while(this.peek().value === "*" || this.peek().value === "/" || this.peek().value === "%"){
      const op = this.eat().value;
      const right = this.parsePrimaryExpression();
      left = {
        kind: ExpressionKind.BinayExpression,
        left,
        right, 
        op,
      }

    }

    return left; 
  }

  private parsePrimaryExpression(): Expression {
    
    switch (this.peek().type) {
      case TokenType.NUMBER:{
        return { kind: ExpressionKind.NumericLiteral, value: parseFloat(this.eat().value) };
      }
        
      case TokenType.IDENTIFIER: {
        return { kind: ExpressionKind.Identifier, symbol: this.eat().value };  
      }
    
      case TokenType.OPEN_PARENTHESIS: {
        this.eat();
        const expr = this.parseExpression();

        if(this.eat().type !== TokenType.CLOSED_PARENTHESIS){
          console.error("Non-matching parenthesis are not allowed");
          process.exit(1);
        }

        return expr;
      }

      default: {
        console.error(`Error!, Could not identify symbol '${this.eat().value}'`);
        process.exit(1);
      }
    }
  }

}


// understanding priority order of different parts of language grammar is important 
// 1. Assignment Expression 
// 2. Additive expression 
// 3. Multiplicative expression 
// 4. Primary expression
