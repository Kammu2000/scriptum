export type NodeType = Program | Statement | Expression;

export type Program = {
  kind: ProgramKind.Program;
  body: Statement[];
} 

// expression is something which has some value and it returns a value
export type Expression = NumericLiteral | Identifier | BinaryExpression | AssignmentExpression | CallExpression; 

// statement does not return any value. it is an action which is performing something 
export type Statement = ExpressionStatement | VariableDeclaration | IfStatement | BlockStatement | WhileStatement | FunctionDeclaration;

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

export type AssignmentExpression = {
  kind: ExpressionKind.AssignmentExpression;
  identifier: string;
  value: Expression;
}

export type CallExpression = {
  kind: ExpressionKind.CallExpression;
}

export type VariableDeclaration = {
  kind: StatementKind.VariableDeclaration;
  identifier: string;
  value?: Expression;
}

export type IfStatement = {
  kind: StatementKind.IfStatement;
  condition: Expression;
  thenBlock: BlockStatement;
  elseBlock?: BlockStatement | IfStatement;
}

export type WhileStatement = {
  kind: StatementKind.WhileStatement;
  condition: Expression;
  body: BlockStatement;
}

export type ExpressionStatement = {
  kind: StatementKind.ExpressionStatement;
  expression: Expression;
}

export type BlockStatement = {
  kind: StatementKind.BlockStatement;
  body: Statement[];
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
  BlockStatement = "BlockStatement",
}

