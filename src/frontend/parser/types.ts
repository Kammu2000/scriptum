export type NodeType = Program | Statement | Expression;

export type Program = {
  kind: ProgramKind.Program;
  body: Statement[];
} 

// expression is something which has some value and it returns a value
export type Expression = NumericLiteral | Identifier | BinaryExpression | CallExpression | AssignmentExpression; 

// statement does not return any value. it is an action which is performing something 
export type Statement = VariableDeclaration | IfStatement | WhileStatement | ExpressionStatement | FunctionDeclaration;

export type NumericLiteral = {
  kind: ExpressionKind.NumericLiteral;
  value: number;
} 

export type Identifier = {
  kind: ExpressionKind.Identifier;
  symbol: string;
} 

export type BinaryExpression = {
  kind: ExpressionKind.BinaryExpression;
  left: Expression;
  right: Expression;
  op: string;
}

export type CallExpression = {
  kind: ExpressionKind.CallExpression;
}

export type AssignmentExpression = {
  kind: ExpressionKind.AssignmentExpression;
}

export type VariableDeclaration = {
  kind: StatementKind.VariableDeclaration;
}

export type IfStatement = {
  kind: StatementKind.IfStatement;
}

export type WhileStatement = {
  kind: StatementKind.WhileStatement;
}

export type ExpressionStatement = {
  kind: StatementKind.ExpressionStatement;
  expression: Expression;
}

export type FunctionDeclaration = {
  kind: StatementKind.FunctionDeclaration;
}

export enum ProgramKind {
  Program = "Program",
} 

export enum ExpressionKind {
  NumericLiteral = "NumericLiteral",
  Identifier = "Identifier", 
  BinaryExpression = "BinaryExpression",
  CallExpression = "CallExpression",
  AssignmentExpression = "AssignmentExpression",
}

export enum StatementKind {
  VariableDeclaration = "VariableDeclaration",
  IfStatement = "IfStatement",
  WhileStatement = "WhileStatement",
  ExpressionStatement = "ExpressionStatement",
  FunctionDeclaration = "FunctionDeclaration",
}

