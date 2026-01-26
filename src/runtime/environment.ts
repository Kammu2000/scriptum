export default class Environment {
  private variables: Map<string, any> = new Map();
  private parent: Environment | null;

  constructor(par: Environment | null) {
    this.variables = new Map();
    this.parent = par; 
  }

  public declareVar(name: string, value: any): void {
    if(this.variables.has(name)){
      throw new Error("Cannot redefine variable");
    }

    this.variables.set(name, value);
    return;
  } 

  public assignVar(name: string, value: any): void {
    const env = this.resolveEnv(name);

    env.variables.set(name, value);
    return;
  }
  
  public resolveEnv(name: string): Environment {
    if(this.variables.has(name))
      return this;

    if (!this.parent) {
      throw new Error(`${name} has not been defined anywhere in program`); 
    }

    return this.parent.resolveEnv(name);
  }

  public lookupVar(name: string): any {
    const env = this.resolveEnv(name);

    return env.variables.get(name);
  }
}

