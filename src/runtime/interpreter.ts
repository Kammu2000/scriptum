import Parser from "../frontend/parser/ast";


const code = "5 + (3 + 4)* )"
const parser = new Parser();

const ast = parser.buildAST(code);
console.log(JSON.stringify(ast));

