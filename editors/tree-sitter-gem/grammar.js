/// <reference types="tree-sitter-cli/dsl" />

module.exports = grammar({
  name: 'gem',

  extras: $ => [
    /\s/,
    $.comment,
  ],

  word: $ => $.identifier,

  conflicts: $ => [
    [$.pattern, $.array],
    [$.pattern, $.table_pair],
  ],

  rules: {
    program: $ => repeat($._statement),

    _statement: $ => choice(
      $.function_definition,
      $.extern_declaration,
      $.load_statement,
      $.export_statement,
      $.let_declaration,
      $.assignment,
      $.if_statement,
      $.while_statement,
      $.for_statement,
      $.match_statement,
      $.receive_block,
      $.return_statement,
      $.break_statement,
      $.continue_statement,
      $._expression,
    ),

    comment: $ => token(seq('#', /.*/)),

    // Declarations

    function_definition: $ => seq(
      'fn',
      field('name', $.identifier),
      $.parameter_list,
      field('body', repeat($._statement)),
      'end',
    ),

    parameter_list: $ => seq(
      '(',
      optional(sep1($._param, ',')),
      ')',
    ),

    _param: $ => choice(
      $.default_param,
      $.identifier,
      $.spread_param,
    ),

    default_param: $ => seq(
      field('name', $.identifier),
      '=',
      field('value', $._expression),
    ),

    spread_param: $ => seq('...', $.identifier),

    extern_declaration: $ => choice(
      $.extern_include,
      $.extern_fn,
    ),

    extern_include: $ => seq('extern', 'include', $._string),

    extern_fn: $ => seq(
      'extern',
      optional('blocking'),
      'fn',
      field('name', $.identifier),
      '(',
      optional(sep1($.typed_param, ',')),
      ')',
      optional(seq('->', $.type)),
    ),

    typed_param: $ => seq(
      field('name', $.identifier),
      ':',
      field('type', $.type),
    ),

    type: $ => choice('Int', 'Float', 'String', 'Bool', 'Nil', 'Ptr', 'Table', 'Fn', 'Bytes'),

    load_statement: $ => seq('load', $._string),

    export_statement: $ => seq('export', sep1($.identifier, ',')),

    let_declaration: $ => seq(
      'let',
      field('name', choice(
        $.identifier,
        $.table_destructure,
        $.array_destructure,
      )),
      '=',
      field('value', $._expression),
    ),

    table_destructure: $ => seq('{', optional(sep1($.identifier, ',')), '}'),
    array_destructure: $ => seq('[', optional(sep1($.identifier, ',')), ']'),

    // Control flow

    if_statement: $ => seq(
      'if',
      field('condition', $._expression),
      optional('then'),
      repeat($._statement),
      repeat($.elif_clause),
      optional($.else_clause),
      'end',
    ),

    elif_clause: $ => seq(
      'elif',
      field('condition', $._expression),
      optional('then'),
      repeat($._statement),
    ),

    else_clause: $ => seq('else', repeat($._statement)),

    while_statement: $ => seq(
      'while',
      field('condition', $._expression),
      repeat($._statement),
      'end',
    ),

    for_statement: $ => choice(
      seq('for', field('key', $.identifier), ',', field('value', $.identifier),
        'in', field('iterable', $._expression),
        repeat($._statement), 'end'),
      seq('for', field('variable', $.identifier),
        'in', field('iterable', $._expression),
        repeat($._statement), 'end'),
      seq('for', field('variable', $.identifier),
        '=', field('start', $._expression), ',', field('stop', $._expression),
        repeat($._statement), 'end'),
    ),

    match_statement: $ => seq(
      'match',
      field('subject', $._expression),
      repeat1($.when_clause),
      optional($.else_clause),
      'end',
    ),

    receive_block: $ => seq(
      'receive',
      repeat1($.when_clause),
      optional($.after_clause),
      'end',
    ),

    when_clause: $ => seq(
      'when',
      field('pattern', $.pattern),
      repeat($._statement),
    ),

    after_clause: $ => seq(
      'after',
      field('timeout', $._expression),
      repeat($._statement),
    ),

    return_statement: $ => prec.right(seq('return', optional($._expression))),
    break_statement: $ => 'break',
    continue_statement: $ => 'continue',

    // Patterns (visible rule for conflict resolution)

    pattern: $ => choice(
      prec.dynamic(2, $.table_pattern),
      prec.dynamic(2, $.array_pattern),
      $._expression,
    ),

    table_pattern: $ => seq('{', sep1($.pattern_pair, ','), '}'),
    pattern_pair: $ => seq(field('key', $.identifier), ':', field('value', $.pattern)),
    array_pattern: $ => seq('[', sep1($.pattern, ','), ']'),

    // Assignment

    assignment: $ => prec.right(-1, seq(
      field('left', choice(
        $.identifier,
        $.member_expression,
        $.subscript_expression,
      )),
      field('operator', choice('=', '+=', '-=', '*=', '/=')),
      field('right', $._expression),
    )),

    // Expressions

    _expression: $ => choice(
      $.binary_expression,
      $.unary_expression,
      $.call_expression,
      $.call_with_block,
      $.member_expression,
      $.subscript_expression,
      $.lambda,
      $.table,
      $.array,
      $._string,
      $.float,
      $.integer,
      $.true,
      $.false,
      $.nil,
      $.identifier,
      $.parenthesized_expression,
    ),

    binary_expression: $ => {
      const table = [
        [1, 'or'],
        [2, 'and'],
        [3, choice('==', '!=')],
        [4, choice('<', '>', '<=', '>=', 'in')],
        [5, choice('+', '-')],
        [6, choice('*', '/', '%')],
      ];
      return choice(
        ...table.map(([precedence, operator]) =>
          prec.left(precedence, seq(
            field('left', $._expression),
            field('operator', operator),
            field('right', $._expression),
          ))
        )
      );
    },

    unary_expression: $ => prec(7, seq(
      field('operator', choice('not', '-')),
      field('operand', $._expression),
    )),

    call_expression: $ => prec.left(8, seq(
      field('function', $._expression),
      '(',
      optional(sep1($._expression, ',')),
      ')',
      optional(field('block', $.do_block)),
    )),

    member_expression: $ => prec.left(9, seq(
      field('object', $._expression),
      '.',
      field('property', $.identifier),
    )),

    subscript_expression: $ => prec.left(9, seq(
      field('object', $._expression),
      '[',
      field('index', $._expression),
      ']',
    )),

    call_with_block: $ => prec.left(8, seq(
      field('function', $.identifier),
      $.do_block,
    )),

    do_block: $ => seq(
      'do',
      optional($.block_parameters),
      repeat($._statement),
      'end',
    ),

    brace_block: $ => seq(
      '{',
      optional($.block_parameters),
      repeat1($._statement),
      '}',
    ),

    block_parameters: $ => seq('|', sep1(choice($.default_param, $.identifier), ','), '|'),

    lambda: $ => seq(
      'fn',
      $.parameter_list,
      repeat($._statement),
      'end',
    ),

    // Literals

    table: $ => seq(
      '{',
      optional(seq(sep1($.table_pair, ','), optional(','))),
      '}',
    ),

    table_pair: $ => seq(
      field('key', choice($.identifier, $._string)),
      ':',
      field('value', $._expression),
    ),

    array: $ => seq(
      '[',
      optional(seq(sep1($._expression, ','), optional(','))),
      ']',
    ),

    parenthesized_expression: $ => seq('(', $._expression, ')'),

    // Strings

    _string: $ => choice(
      $.single_string,
      $.double_string,
      $.triple_single_string,
      $.triple_double_string,
    ),

    single_string: $ => seq(
      "'",
      repeat(choice(
        alias($._single_string_content, $.string_content),
        $.escape_sequence,
      )),
      "'",
    ),

    double_string: $ => seq(
      '"',
      repeat(choice(
        $.string_content,
        $.escape_sequence,
        $.interpolation,
      )),
      '"',
    ),

    triple_single_string: $ => token(seq(
      "'''",
      repeat(choice(
        /[^'\\]/,
        seq('\\', /./),
        seq("'", /[^']/),
        seq("''", /[^']/),
      )),
      "'''",
    )),

    triple_double_string: $ => token(seq(
      '"""',
      repeat(choice(
        /[^"\\]/,
        seq('\\', /./),
        seq('"', /[^"]/),
        seq('""', /[^"]/),
      )),
      '"""',
    )),

    string_content: $ => token.immediate(prec(1, /[^"\\{]+/)),
    _single_string_content: $ => token.immediate(prec(1, /[^'\\]+/)),

    interpolation: $ => seq(
      token.immediate('{'),
      $._expression,
      '}',
    ),

    escape_sequence: $ => token.immediate(seq('\\', /[nrt0\\'"{}]/)),

    float: $ => token(seq(/[0-9]+/, '.', /[0-9]+/)),
    integer: $ => /[0-9]+/,

    true: $ => 'true',
    false: $ => 'false',
    nil: $ => 'nil',

    identifier: $ => /[a-zA-Z_][a-zA-Z0-9_]*/,
  },
});

function sep1(rule, separator) {
  return seq(rule, repeat(seq(separator, rule)));
}
