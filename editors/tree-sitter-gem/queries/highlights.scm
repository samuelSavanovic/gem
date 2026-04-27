; Keywords
"fn" @keyword.function
"end" @keyword
"let" @keyword
"do" @keyword
"then" @keyword
"extern" @keyword
"blocking" @keyword

"load" @keyword.control.import
"include" @keyword.control.import

"if" @keyword.control.conditional
"elif" @keyword.control.conditional
"else" @keyword.control.conditional
"match" @keyword.control.conditional
"when" @keyword.control.conditional

"while" @keyword.control.repeat
"for" @keyword.control.repeat
"in" @keyword.control.repeat
(break_statement) @keyword.control.repeat
(continue_statement) @keyword.control.repeat

"return" @keyword.control.return

"receive" @keyword
"after" @keyword

"and" @keyword.operator
"or" @keyword.operator
"not" @keyword.operator

; Literals
(true) @constant.builtin.boolean
(false) @constant.builtin.boolean
(nil) @constant.builtin
(integer) @constant.numeric.integer
(float) @constant.numeric.float

; Strings
(single_string) @string
(double_string) @string
(triple_single_string) @string
(triple_double_string) @string
(string_content) @string
(escape_sequence) @constant.character.escape
(interpolation
  "{" @punctuation.special
  "}" @punctuation.special)

; Types (in extern declarations)
(type) @type.builtin

; Functions
(function_definition
  name: (identifier) @function)

(extern_fn
  name: (identifier) @function)

(lambda "fn" @keyword.function)

(call_expression
  function: (identifier) @function.call)

(call_expression
  function: (member_expression
    property: (identifier) @function.method))

(call_with_block
  function: (identifier) @function.call)

; Builtin functions
((call_expression
  function: (identifier) @function.builtin)
  (#match? @function.builtin "^(print|error|len|type|to_string|to_int|to_float|push|pcall|keys|str_replace|has_key|substr|chr|ord|buf_new|buf_push|buf_str|spawn|send|receive|monitor|spawn_monitor|register|whereis|time_ms|kill|link|unlink|spawn_link|process_flag|make_ref|send_after|cancel_timer|process_info|read_file|write_file|delete|pop|values|eprint|exit|argv|sort|floor|ceil|round|abs|pow|sqrt|random|append_file|getenv|input|insert|remove_at|band|bor|bxor|bnot|bshl|bshr|file_exists|dirname|path_join|normalize_path|remove_file|mkdir|list_dir|is_dir|exec|tcp_connect|tcp_listen|tcp_accept|tcp_read|tcp_write|tcp_close)$"))

((call_with_block
  function: (identifier) @function.builtin)
  (#match? @function.builtin "^(spawn|send|receive|monitor|spawn_monitor|register)$"))

; Parameters
(parameter_list (identifier) @variable.parameter)
(spread_param (identifier) @variable.parameter)
(typed_param name: (identifier) @variable.parameter)
(block_parameters (identifier) @variable.parameter)

"..." @punctuation.special

; Variable declarations
(let_declaration
  name: (identifier) @variable)

; Comments
(comment) @comment.line

; Operators
(binary_expression operator: _ @operator)
(unary_expression operator: _ @operator)
(assignment operator: _ @operator)
"->" @operator

; Punctuation
"(" @punctuation.bracket
")" @punctuation.bracket
"[" @punctuation.bracket
"]" @punctuation.bracket
"{" @punctuation.bracket
"}" @punctuation.bracket

"." @punctuation.delimiter
"," @punctuation.delimiter
":" @punctuation.delimiter
"|" @punctuation.delimiter
