export type NodeType = Program | Statement | Expression;

export type Program = {
  kind: ProgramKind.Program;
  body: Statement[];
} 

// expression is something which has some value and it returns a value
export type Expression = NumericLiteral | Identifier | BinaryExpression | AssignmentExpression | CallExpression; 

// statement does not return any value. it is an action which is performing something 
export type Statement = ExpressionStatement | VariableDeclaration | IfStatement | BlockStatement | WhileStatement | ReturnStatement | FunctionDeclaration;

// expression types 
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
  op: string;
  left: Identifier;
  right: Expression;
}

export type CallExpression = {
  kind: ExpressionKind.CallExpression;
  callee: Expression;
  args: Expression[];
}

// statement types
export type VariableDeclaration = {
  kind: StatementKind.VariableDeclaration;
  id: Identifier;
  init?: Expression;
}

export type IfStatement = {
  kind: StatementKind.IfStatement;
  test: Expression;
  thenBlock: BlockStatement;
  elseBlock?: BlockStatement | IfStatement;
}

export type WhileStatement = {
  kind: StatementKind.WhileStatement;
  test: Expression;
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

export type ReturnStatement = {
  kind: StatementKind.ReturnStatement;
  argument?: Expression;
}

export type FunctionDeclaration = {
  kind: StatementKind.FunctionDeclaration;
  id: Identifier;
  params: Identifier[],
  body: BlockStatement;
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
  ReturnStatement = "ReturnStatement",
  FunctionDeclaration = "FunctionDeclaration",
  BlockStatement = "BlockStatement",
}

