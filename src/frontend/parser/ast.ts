// imports 
import { tokenize } from "../lexer/tokenize";
import { Token, TokenType } from "../lexer/types";
import { Expression, Program, Statement, ProgramKind, ExpressionKind, StatementKind, BlockStatement, ExpressionStatement, IfStatement, VariableDeclaration } from "./types";

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

  next(): Token {
    return this.tokens[this.pos + 1];
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
    switch(this.peek().type){
      case TokenType.LET: {
        return this.parseVarDeclarationStatement();
      } 

      case TokenType.IF: {
        return this.parseIfStatement();
      }
      
      case TokenType.OPEN_BRACES: {
        return this.parseBlockStatement();
      }

      default: 
        return this.parseExpressionStatement();
    }
  }
  
  private parseVarDeclarationStatement(): VariableDeclaration {
    this.eat(); // eat let token 
    const identifier = this.eat().value;
    let value;

    if(this.peek().type === TokenType.EQUALS){
      this.eat();
      value = this.parseExpression();
    }

    return { kind: StatementKind.VariableDeclaration, identifier: identifier, value };      
  }

  private parseIfStatement(): IfStatement {
    this.eat(); // eat if token

    if(this.eat().type !== TokenType.OPEN_PARENTHESIS){
      throw new Error("Expected open parenthesis while starting condition of if statement");
    }

    const condition = this.parseExpression();

    if(this.eat().type !== TokenType.CLOSED_PARENTHESIS){
      throw new Error("Expected closed parenthesis while ending condition of if statement");
    }
    
    if(this.peek().type !== TokenType.OPEN_BRACES){
      throw new Error("No open braces found at staring of if statement");
    }

    const thenBlock = this.parseBlockStatement(); 
    let elseBlock;
    
    if(this.peek().type === TokenType.ELSE){
      this.eat();
      if(this.peek().type === TokenType.IF){
        elseBlock = this.parseIfStatement();
      }
      else if(this.peek().type === TokenType.OPEN_BRACES){
        elseBlock = this.parseBlockStatement();
      }
      else {
        throw new Error("else statement should have block or a new if statement");
      }
    }

    return { kind: StatementKind.IfStatement, condition, thenBlock, elseBlock };
  }


  private parseBlockStatement(): BlockStatement {
    this.eat();

    const body: Statement[] = [];

    while(!this.endOf() && this.peek().type !== TokenType.CLOSE_BRACES){
      const newStmt = this.parseStatement();
      body.push(newStmt);
    }
    
    if(this.peek().type !== TokenType.CLOSE_BRACES){
      throw new Error("No closed braces found at the end of if statement");
    }
    else {
      this.eat();
    }
    
    return { kind: StatementKind.BlockStatement, body };
  }

private parseExpressionStatement(): ExpressionStatement {
    const expr = this.parseExpression();
    return { kind: StatementKind.ExpressionStatement, expression: expr };
  }

  private parseExpression(): Expression {
    return this.parseAssignmentExpression();
  }

  private parseAssignmentExpression(): Expression {
    
    if(this.next().type === TokenType.EQUALS){
      if(this.peek().type !== TokenType.IDENTIFIER){
        throw new Error("Invalid syntax, cannot assign a expression to anything other than identifier");
      }

      const identifier = this.eat().value;
      this.eat(); // eat equals token 
      return { kind: ExpressionKind.AssignmentExpression, identifier, value: this.parseExpression() };
    }

    return this.parseAdditiveExpression(); 
  }

  private parseAdditiveExpression(): Expression {
    let left = this.parseMultiplicativeExpression();

    while(this.peek().value === "+" || this.peek().value === "-"){
      const op = this.eat().value;
      const right = this.parseMultiplicativeExpression();

      left = {
        kind: ExpressionKind.BinaryExpression,
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
        kind: ExpressionKind.BinaryExpression,
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
// 1. VariableDeclaration
// 1. Assignment Expression 
// 2. Additive expression 
// 3. Multiplicative expression 
// 4. Primary expression
