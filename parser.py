import ast
import token

LOWEST      = 0  #
ASSIGN      = 1  # = or :=
QUESTION    = 2  # ?
OR          = 3  # ||
AND         = 4  # &&
BIT_OR      = 5  # |
BIT_AND     = 6  # &
EQUALS      = 7  # == or !==
LESSGREATER = 8  # < or > or <= or >=
SUM         = 9  # + or -
PRODUCT     = 10 # * or / or %
EXP         = 11 # ** or //
PREFIX      = 12 # -x
METHOD_CALL = 13 # instance: pattern
DOT         = 14 # x.y

precedences = {
    token.EQ:      EQUALS,
    token.N_EQ:    EQUALS,
    token.LT:      LESSGREATER,
    token.GT:      LESSGREATER,
    token.PLUS:    SUM,
    token.MINUS:   SUM,
    token.STAR:    PRODUCT,
    token.SLASH:   PRODUCT,
    token.ASSIGN:  ASSIGN,
    token.DECLARE: ASSIGN,
    token.OR:      OR,
    token.AND:     AND,
    token.B_OR:    BIT_OR,
    token.B_AND:   BIT_AND,
    token.EXP:     EXP,
    token.F_DIV:   EXP,
    token.MOD:     PRODUCT,
    token.LTE:     LESSGREATER,
    token.GTE:     LESSGREATER,
    token.Q_MARK:  QUESTION,
    token.COLON:   METHOD_CALL,
    token.DOT:     DOT
}

arg_tokens = [
    token.ID,
    token.NUM,
    token.NULL,
    token.TRUE,
    token.FALSE,
    token.STR,
    token.PARAM,
    token.LPAREN,
    token.LSQUARE,
    token.LBRACE
]

class Parser(object):
    """parses a stream of tokens into an abstract syntax tree (AST)"""
    def __init__(self, tokens):
        self.tokens   = tokens
        self.errors   = [] # [(msg, start, end)]
        self.cur_tok  = None
        self.peek_tok = None

        self.prefixes = {
            # Literals
            token.ID:      self.parse_id,
            token.NUM:     self.parse_num,
            token.TRUE:    self.parse_bool,
            token.FALSE:   self.parse_bool,
            token.NULL:    self.parse_null,
            token.LSQUARE: self.parse_array_or_object,
            token.STR:     self.parse_string,
            token.CHAR:    self.parse_char,

            # Prefix operators
            token.MINUS: self.parse_prefix,
            token.PLUS:  self.parse_prefix,

            # Constructs
            token.LPAREN: self.parse_grouped_expr,
            token.IF:     self.parse_if_expr,
            token.BSLASH: self.parse_function_call,
            token.LBRACE: self.parse_block_literal,
            token.WHILE:  self.parse_while_loop,
            token.FOR:    self.parse_for_loop
        }

        self.infixes  = {
            token.PLUS:    self.parse_infix,
            token.MINUS:   self.parse_infix,
            token.STAR:    self.parse_infix,
            token.SLASH:   self.parse_infix,
            token.EQ:      self.parse_infix,
            token.N_EQ:    self.parse_infix,
            token.LT:      self.parse_infix,
            token.GT:      self.parse_infix,
            token.OR:      self.parse_infix,
            token.AND:     self.parse_infix,
            token.B_OR:    self.parse_infix,
            token.B_AND:   self.parse_infix,
            token.EXP:     self.parse_infix,
            token.F_DIV:   self.parse_infix,
            token.MOD:     self.parse_infix,
            token.LTE:     self.parse_infix,
            token.GTE:     self.parse_infix,
            token.Q_MARK:  self.parse_infix,
            token.ASSIGN:  self.parse_assign_expr,
            token.DECLARE: self.parse_declare_expr,
            token.DOT:     self.parse_dot_expr,
            token.COLON:   self.parse_method_call
        }

        self.next()
        self.next()

    def peek_precedence(self):
        return precedences.get(self.peek_tok.type, LOWEST)

    def cur_precedence(self):
        return precedences.get(self.cur_tok.type, LOWEST)

    def err(self, msg, start = None, end = None):
        if start == None:
            start = self.cur_tok.start

        if end == None:
            end = self.cur_tok.end

        error = (msg, start, end)
        self.errors.append(error)

    def peek_err(self, t):
        if type(t) == type([]):
            msg = "expected any of [%s], but got %s" % (
                ("".join(str(ty) + ", " for ty in t))[:-2],
                self.peek_tok.type
            )
        else:
            msg = "expected %s, but got %s" % (t, self.peek_tok.type)

        self.err(msg, self.peek_tok.start, self.peek_tok.end)

    def cur_err(self, t):
        if type(t) == type([]):
            msg = "expected any of [%s], but got %s" % (
                ("".join(str(ty) + ", " for ty in t))[:-2],
                self.cur_tok.type
            )
        else:
            msg = "expected %s, but got %s" % (t, self.cur_tok.type)

        self.err(msg)

    def no_prefix_fn_error(self, t):
        msg = "unexpected token: %s" % t
        self.err(msg)

    def print_error(self, index = 0):
        msg, start, end = self.errors[index]

        print('[%s:%s] to [%s:%s] -- %s' % (
            start[0], start[1],
            end[0], end[1],
            msg
        ))

    def print_errors(self):
        for i in range(len(self.errors)):
            self.print_error(i)

    def next(self):
        self.cur_tok = self.peek_tok
        self.peek_tok = next(self.tokens)

        if self.peek_tok.type == token.ILLEGAL:
            self.err("illegal token found: `%s`" % self.peek_tok.literal, self.peek_tok.start, self.peek_tok.end)

    def parse_program(self):
        prog = ast.Program([])

        while not self.cur_is(token.EOF):
            stmt = self.parse_stmt()

            if stmt != None:
                prog.statements.append(stmt)

            self.next()

        return prog

    def parse_stmt(self):
        stmt = None

        if self.cur_is(token.SEMI):
            return None
        elif self.cur_is(token.RETURN):
            stmt = self.parse_return_stmt()
        elif self.cur_is(token.DEF):
            stmt = self.parse_def_stmt()
        elif self.cur_is(token.NEXT):
            stmt = self.parse_next_stmt()
        elif self.cur_is(token.BREAK):
            stmt = self.parse_break_stmt()
        elif self.cur_is(token.CLASS):
            stmt = self.parse_class_declaration()
        else:
            stmt = self.parse_expr_stmt()

        if not self.expect(token.SEMI):
            return None

        return stmt

    def parse_block_statement(self):
        block = ast.BlockStatement(self.cur_tok, [])

        self.next()

        while not self.cur_is(token.RBRACE) and not self.cur_is(token.EOF):
            stmt = self.parse_stmt()

            if stmt != None:
                block.statements.append(stmt)

            self.next()

        return block

    def parse_expr_stmt(self):
        return ast.ExpressionStatement(self.cur_tok, self.parse_expr(LOWEST))

    def parse_return_stmt(self):
        if self.peek_tok.type == token.SEMI:
            return ast.ReturnStatement(self.cur_tok, None)

        stmt = ast.ReturnStatement(self.cur_tok, None)
        self.next()
        stmt.value = self.parse_expr(LOWEST)

        return stmt

    def parse_next_stmt(self):
        return ast.NextStatement(self.cur_tok)

    def parse_break_stmt(self):
        return ast.BreakStatement(self.cur_tok)

    def parse_expr(self, precedence):
        prefix = self.prefixes.get(self.cur_tok.type, None)

        if prefix == None:
            self.no_prefix_fn_error(self.cur_tok.type)
            return None

        left = prefix()

        while not self.peek_is(token.SEMI) and precedence < self.peek_precedence():
            infix = self.infixes.get(self.peek_tok.type, None)

            if infix == None:
                return left

            self.next()

            left = infix(left)

        return left

    def parse_id(self, can_be_fn = True):
        if can_be_fn and self.peek_in(arg_tokens):
            return self.parse_function_call(ast.Identifier(self.cur_tok))

        return ast.Identifier(self.cur_tok)

    def parse_num(self):
        lit = ast.Number(self.cur_tok, None)

        try:
            lit.value = float(self.cur_tok.literal)
        except ValueError:
            msg = "could not parse %s as a number" % self.cur_tok.literal
            self.err(msg)
            return None

        return lit

    def parse_bool(self):
        return ast.Boolean(self.cur_tok, self.cur_tok.type == token.TRUE)

    def parse_null(self):
        return ast.Null(self.cur_tok)

    def parse_string(self):
        return ast.String(self.cur_tok)

    def parse_char(self):
        return ast.Char(self.cur_tok)

    def parse_prefix(self):
        expr = ast.PrefixExpression(self.cur_tok, self.cur_tok.literal, None)

        self.next()

        expr.right = self.parse_expr(PREFIX)

        return expr

    def parse_grouped_expr(self):
        self.next()

        if self.cur_is(token.RPAREN):
            expr = ast.Tuple(self.cur_tok, [])

            return expr

        expr = self.parse_expr(LOWEST)

        if self.peek_is(token.COMMA):
            self.next()
            self.next()

            expr = ast.Tuple(expr.token, [expr] + self.parse_expr_list(token.RPAREN))

        if type(expr) != ast.Tuple and not self.expect(token.RPAREN):
            return None

        return expr

    def parse_array_or_object(self):
        self.next()

        if self.peek_is(token.COLON) or self.cur_is(token.COLON):
            pairs = self.parse_expr_pairs(token.RSQUARE)
            return ast.Object(self.cur_tok, pairs)
        else:
            return ast.Array(self.cur_tok, self.parse_expr_list(token.RSQUARE))

    def parse_function_call(self, first = None):
        expr = ast.FunctionCall(self.cur_tok, [])

        if first != None:
            expr.pattern.append(first)

        # If the current token is a valid arg token, or a keyword
        while self.peek_in(arg_tokens) or self.peek_in(token.keywords.values()):
            self.next()

            if self.cur_in(token.keywords.values()):
                expr.pattern.append(ast.Identifier(self.cur_tok))
                continue

            arg = lambda val: ast.Argument(self.cur_tok, val)

            handlers = {
                token.ID:      lambda: ast.Identifier(self.cur_tok),
                token.LPAREN:  lambda: arg(self.parse_grouped_expr()),
                token.NUM:     lambda: arg(self.parse_num()),
                token.NULL:    lambda: arg(self.parse_null()),
                token.TRUE:    lambda: arg(self.parse_bool()),
                token.FALSE:   lambda: arg(self.parse_bool()),
                token.STR:     lambda: arg(self.parse_string()),
                token.PARAM:   lambda: arg(ast.Identifier(self.cur_tok)),
                token.LSQUARE: lambda: arg(self.parse_array_or_object()),
                token.LBRACE:  lambda: arg(self.parse_block_literal()),
            }

            handler = handlers[self.cur_tok.type]
            expr.pattern.append(handler())

        if len(expr.pattern) == 0:
            self.err("expected at least one item in a pattern")
            return None

        return expr

    def parse_if_expr(self):
        expr = ast.IfExpression(self.cur_tok, None, None, None)

        if not self.expect(token.LPAREN):
            return None

        self.next()
        expr.condition = self.parse_expr(LOWEST)

        if not self.expect(token.RPAREN):
            return None

        if not self.expect(token.LBRACE):
            return None

        expr.consequence = self.parse_block_statement()

        if self.peek_is(token.ELSE):
            self.next()

            if not self.expect(token.LBRACE):
                return None

            expr.alternative = self.parse_block_statement()
        elif self.peek_is(token.ELIF):
            self.next()

            expr.alternative = ast.BlockStatement(
                self.cur_tok,
                [ast.ExpressionStatement(self.cur_tok, self.parse_if_expr())]
            )

        return expr

    def parse_infix(self, left):
        expr = ast.InfixExpression(self.cur_tok, self.cur_tok.literal, left, None)

        precedence = self.cur_precedence()
        self.next()
        expr.right = self.parse_expr(precedence)

        return expr

    def parse_assign_expr(self, left):
        expr = ast.AssignExpression(self.cur_tok, left, None)

        self.next()
        expr.value = self.parse_expr(LOWEST)

        return expr

    def parse_declare_expr(self, left):
        expr = ast.DeclareExpression(self.cur_tok, left, None)

        self.next()
        expr.value = self.parse_expr(LOWEST)

        return expr

    def parse_dot_expr(self, left):
        expr = ast.DotExpression(self.cur_tok, left, None)

        self.next()
        expr.right = self.parse_expr(DOT)

        return expr
        
    def parse_method_call(self, left):
        expr = ast.MethodCall(self.cur_tok, left, [])
        
        while self.peek_in(arg_tokens) or self.peek_in(token.keywords.values()):
            self.next()

            if self.cur_in(token.keywords.values()):
                expr.pattern.append(ast.Identifier(self.cur_tok))
                continue

            arg = lambda val: ast.Argument(self.cur_tok, val)

            handlers = {
                token.ID:      lambda: ast.Identifier(self.cur_tok),
                token.LPAREN:  lambda: arg(self.parse_grouped_expr()),
                token.NUM:     lambda: arg(self.parse_num()),
                token.NULL:    lambda: arg(self.parse_null()),
                token.TRUE:    lambda: arg(self.parse_bool()),
                token.FALSE:   lambda: arg(self.parse_bool()),
                token.STR:     lambda: arg(self.parse_string()),
                token.PARAM:   lambda: arg(ast.Identifier(self.cur_tok)),
                token.LSQUARE: lambda: arg(self.parse_array_or_object()),
                token.LBRACE:  lambda: arg(self.parse_block_literal()),
            }

            handler = handlers[self.cur_tok.type]
            expr.pattern.append(handler())

        if len(expr.pattern) == 0:
            self.err("expected at least one item in a pattern")
            return None
        
        return expr

    def parse_block_literal(self):
        expr = ast.BlockLiteral(self.cur_tok, None, None)

        if self.peek_is(token.B_OR):
            self.next()
            expr.params = self.parse_params(token.B_OR)

            if not self.expect(token.ARROW):
                return None

        expr.body = self.parse_block_statement()

        return expr

    def parse_while_loop(self):
        expr = ast.WhileLoop(self.cur_tok, None, None)

        if not self.expect(token.LPAREN):
            return None

        self.next()
        expr.condition = self.parse_expr(LOWEST)

        if not self.expect(token.RPAREN):
            return None

        if not self.expect(token.LBRACE):
            return None

        expr.body = self.parse_block_statement()

        return expr

    def parse_for_loop(self):
        expr = ast.ForLoop(self.cur_tok, None, None, None)

        if not self.expect(token.LPAREN):
            return None

        self.next()
        expr.var = self.parse_id(True)

        if not self.expect(token.COLON):
            return None

        self.next()

        expr.collection = self.parse_expr(LOWEST)

        if not self.expect(token.RPAREN):
            return None

        if not self.expect(token.LBRACE):
            return None

        expr.body = self.parse_block_statement()

        return expr

    def parse_class_declaration(self):
        stmt = ast.ClassStatement(self.cur_tok, None, [], None)

        if not self.expect(token.ID):
            return None

        stmt.name = self.parse_id(False)

        if self.peek_is(token.EXTENDS):
            self.next()
            self.next()

            stmt.parent = self.parse_id(False)

        if not self.expect(token.LBRACE):
            return None

        self.next()
        
        if self.cur_is(token.RBRACE):
            return stmt

        while self.cur_in([token.INIT, token.DEF]):
            if self.cur_is(token.INIT):
                stmt.methods.append(self.parse_init_stmt())
            elif self.cur_is(token.DEF):
                stmt.methods.append(self.parse_def_stmt())
                
            if not self.expect(token.SEMI):
                return None

            if self.peek_in([token.INIT, token.DEF]):
                self.next()

        if not self.expect(token.RBRACE):
            return None

        return stmt

    def parse_init_stmt(self):
        stmt = ast.InitDefinition(self.cur_tok, [], None)

        self.next()

        stmt.pattern = self.parse_pattern_call(token.LBRACE)

        stmt.body = self.parse_block_statement()

        if len(stmt.pattern) == 0:
            self.err("expected at least one item in a pattern")
            return None

        return stmt

    def parse_def_stmt(self):
        stmt = ast.FunctionDefinition(self.cur_tok, [], None)

        self.next()

        stmt.pattern = self.parse_pattern_call(token.LBRACE)

        stmt.body = self.parse_block_statement()

        if len(stmt.pattern) == 0:
            self.err("expected at least one item in a pattern")
            return None

        return stmt

    def parse_pattern_call(self, end):
        pattern = []
        
        while not self.cur_is(end):
            tok = self.cur_tok
            
            if not self.expect_cur_any([token.ID, token.PARAM] + list(token.keywords.values())):
                return None
                
            if tok.type == token.ID or tok.type in token.keywords.values():
                pattern.append(ast.Identifier(tok))
            else:
                pattern.append(ast.Parameter(tok, tok.literal))
                
        return pattern

    def parse_expr_list(self, end):
        exprs = []

        if self.cur_is(end):
            return exprs

        exprs.append(self.parse_expr(LOWEST))

        while self.peek_is(token.COMMA):
            self.next()
            self.next()
            exprs.append(self.parse_expr(LOWEST))

        if not self.expect(end):
            return None

        return exprs

    def parse_expr_pairs(self, end):
        pairs = {}

        if self.cur_is(token.COLON):
            self.next()
            return pairs

        key, val = self.parse_pair()
        pairs[key] = val

        while self.peek_is(token.COMMA):
            self.next()
            self.next()
            key, val = self.parse_pair()
            pairs[key] = val

        if not self.expect(end):
            return None

        return pairs

    def parse_pair(self):
        key = self.parse_expr(LOWEST)

        if not self.expect(token.COLON):
            return None

        self.next()

        value = self.parse_expr(LOWEST)

        return key, value

    def parse_params(self, end):
        params = []

        if self.peek_is(end):
            self.next()
            return params

        self.next()
        params.append(self.parse_id(False))

        while self.peek_is(token.COMMA):
            self.next()
            self.next()
            params.append(self.parse_id(False))

        if not self.expect(end):
            return None

        return params

    def cur_is(self, t):
        return self.cur_tok.type == t

    def peek_is(self, t):
        return self.peek_tok.type == t

    def cur_in(self, t):
        return self.cur_tok.type in t

    def peek_in(self, t):
        return self.peek_tok.type in t

    def expect(self, t):
        if self.peek_is(t):
            self.next()
            return True
        else:
            self.peek_err(t)
            return False

    def expect_cur_any(self, ts):
        if self.cur_in(ts):
            self.next()
            return True
        else:
            self.cur_err(ts)
            return False

    def expect_peek_any(self, ts):
        if self.peek_in(ts):
            self.next()
            return True
        else:
            self.peek_err(ts)
            return False
