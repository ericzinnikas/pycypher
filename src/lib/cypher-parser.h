/* vi:set ts=4 sw=4 expandtab:
 *
 * Copyright 2016, Chris Leishman (http://github.com/cleishm)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * @file cypher-parser.h
 */
#ifndef CYPHER_PARSER_H
#define CYPHER_PARSER_H

#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>

#if __GNUC__ > 3
#define __cypherlang_pure __attribute__((pure))
#define __cypherlang_malloc __attribute__((malloc))
#define __cypherlang_must_check __attribute__((warn_unused_result))
#define __cypherlang_format(string_index, first) \
        __attribute__((format (printf, string_index, first)))
#else
#define __cypherlang_pure /*pure*/
#define __cypherlang_malloc /*malloc*/
#define __cypherlang_must_check /*must check*/
#define __cypherlang_format(string_index, first) /*format*/
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)


/*
 * =====================================
 * version
 * =====================================
 */

/**
 * The version string for libcypher-parser.
 */
__cypherlang_pure
const char *libcypher_parser_version(void);


/*
 * =====================================
 * colorization
 * =====================================
 */

struct cypher_parser_colorization
{
    const char *normal[2];
    const char *error[2];
    const char *error_token[2];
    const char *error_message[2];
    const char *ast_ordinal[2];
    const char *ast_range[2];
    const char *ast_indent[2];
    const char *ast_type[2];
    const char *ast_desc[2];
};

/** Colorization rules for uncolorized output. */
extern const struct cypher_parser_colorization *cypher_parser_no_colorization;
/** Colorization rules for ANSI terminal output. */
extern const struct cypher_parser_colorization *cypher_parser_ansi_colorization;


/*
 * =====================================
 * abstract syntax tree
 * =====================================
 */

/**
 * An abstract syntax tree node.
 */
typedef struct cypher_astnode cypher_astnode_t;

/**
 * A cypher AST node type.
 */
typedef uint8_t cypher_astnode_type_t;

/** Type for an AST statement node. */
extern const cypher_astnode_type_t CYPHER_AST_STATEMENT;
/** Type for an AST statement option node. */
extern const cypher_astnode_type_t CYPHER_AST_STATEMENT_OPTION;
/** Type for an AST `CYPHER` option node. */
extern const cypher_astnode_type_t CYPHER_AST_CYPHER_OPTION;
/** Type for an AST `CYPHER` option parameter node. */
extern const cypher_astnode_type_t CYPHER_AST_CYPHER_OPTION_PARAM;
/** Type for an AST `EXPLAIN` option node. */
extern const cypher_astnode_type_t CYPHER_AST_EXPLAIN_OPTION;
/** Type for an AST `PROFILE` option node. */
extern const cypher_astnode_type_t CYPHER_AST_PROFILE_OPTION;
/** Type for an AST schema command node. */
extern const cypher_astnode_type_t CYPHER_AST_SCHEMA_COMMAND;
/** Type for an AST `CREATE INDEX` node. */
extern const cypher_astnode_type_t CYPHER_AST_CREATE_NODE_PROP_INDEX;
/** Type for an AST `DROP INDEX` node. */
extern const cypher_astnode_type_t CYPHER_AST_DROP_NODE_PROP_INDEX;
/** Type for an AST create node property constraint node. */
extern const cypher_astnode_type_t CYPHER_AST_CREATE_NODE_PROP_CONSTRAINT;
/** Type for an AST drop node property constraint node. */
extern const cypher_astnode_type_t CYPHER_AST_DROP_NODE_PROP_CONSTRAINT;
/** Type for an AST create rel property constraint node. */
extern const cypher_astnode_type_t CYPHER_AST_CREATE_REL_PROP_CONSTRAINT;
/** Type for an AST drop rel property constraint node. */
extern const cypher_astnode_type_t CYPHER_AST_DROP_REL_PROP_CONSTRAINT;
/** Type for an AST query node. */
extern const cypher_astnode_type_t CYPHER_AST_QUERY;
/** Type for an AST query option node. */
extern const cypher_astnode_type_t CYPHER_AST_QUERY_OPTION;
/** Type for an AST `USING PERIODIC COMMIT` clause node. */
extern const cypher_astnode_type_t CYPHER_AST_USING_PERIODIC_COMMIT;
/** Type for an AST query clause node. */
extern const cypher_astnode_type_t CYPHER_AST_QUERY_CLAUSE;
/** Type for an AST `LOAD CSV` clause node. */
extern const cypher_astnode_type_t CYPHER_AST_LOAD_CSV;
/** Type for an AST `START` clause node. */
extern const cypher_astnode_type_t CYPHER_AST_START;
/** Type for an AST start point node. */
extern const cypher_astnode_type_t CYPHER_AST_START_POINT;
/** Type for an AST node-index lookup node. */
extern const cypher_astnode_type_t CYPHER_AST_NODE_INDEX_LOOKUP;
/** Type for an AST node-index query node. */
extern const cypher_astnode_type_t CYPHER_AST_NODE_INDEX_QUERY;
/** Type for an AST node-by-id-lookup node. */
extern const cypher_astnode_type_t CYPHER_AST_NODE_ID_LOOKUP;
/** Type for an AST all-nodes-scan node. */
extern const cypher_astnode_type_t CYPHER_AST_ALL_NODES_SCAN;
/** Type for an AST rel-index lookup node. */
extern const cypher_astnode_type_t CYPHER_AST_REL_INDEX_LOOKUP;
/** Type for an AST rel-index query node. */
extern const cypher_astnode_type_t CYPHER_AST_REL_INDEX_QUERY;
/** Type for an AST rel-by-id-lookup node. */
extern const cypher_astnode_type_t CYPHER_AST_REL_ID_LOOKUP;
/** Type for an AST all-rels-scan node. */
extern const cypher_astnode_type_t CYPHER_AST_ALL_RELS_SCAN;
/** Type for an AST `MATCH` clause node. */
extern const cypher_astnode_type_t CYPHER_AST_MATCH;
/** Type for an AST match hint node. */
extern const cypher_astnode_type_t CYPHER_AST_MATCH_HINT;
/** Type for an AST `USING INDEX` hint node. */
extern const cypher_astnode_type_t CYPHER_AST_USING_INDEX;
/** Type for an AST `USING JOIN ON` hint node. */
extern const cypher_astnode_type_t CYPHER_AST_USING_JOIN;
/** Type for an AST `USING SCAN` hint node. */
extern const cypher_astnode_type_t CYPHER_AST_USING_SCAN;
/** Type for an AST `MERGE` clause node. */
extern const cypher_astnode_type_t CYPHER_AST_MERGE;
/** Type for an AST merge action node. */
extern const cypher_astnode_type_t CYPHER_AST_MERGE_ACTION;
/** Type for an AST `ON MATCH` action node. */
extern const cypher_astnode_type_t CYPHER_AST_ON_MATCH;
/** Type for an AST `ON CREATE` action node. */
extern const cypher_astnode_type_t CYPHER_AST_ON_CREATE;
/** Type for an AST `CREATE` clause node. */
extern const cypher_astnode_type_t CYPHER_AST_CREATE;
/** Type for an AST `SET` clause node. */
extern const cypher_astnode_type_t CYPHER_AST_SET;
/** Type for an AST set item node. */
extern const cypher_astnode_type_t CYPHER_AST_SET_ITEM;
/** Type for an AST set property node. */
extern const cypher_astnode_type_t CYPHER_AST_SET_PROPERTY;
/** Type for an AST set all properties node. */
extern const cypher_astnode_type_t CYPHER_AST_SET_ALL_PROPERTIES;
/** Type for an AST merge properties node. */
extern const cypher_astnode_type_t CYPHER_AST_MERGE_PROPERTIES;
/** Type for an AST set labels node. */
extern const cypher_astnode_type_t CYPHER_AST_SET_LABELS;
/** Type for an AST `DELETE` clause node. */
extern const cypher_astnode_type_t CYPHER_AST_DELETE;
/** Type for an AST `REMOVE` clause node. */
extern const cypher_astnode_type_t CYPHER_AST_REMOVE;
/** Type for an AST remove item node. */
extern const cypher_astnode_type_t CYPHER_AST_REMOVE_ITEM;
/** Type for an AST remove labels node. */
extern const cypher_astnode_type_t CYPHER_AST_REMOVE_LABELS;
/** Type for an AST remove property node. */
extern const cypher_astnode_type_t CYPHER_AST_REMOVE_PROPERTY;
/** Type for an AST `FOREACH` clause node. */
extern const cypher_astnode_type_t CYPHER_AST_FOREACH;
/** Type for an AST `WITH` clause node. */
extern const cypher_astnode_type_t CYPHER_AST_WITH;
/** Type for an AST `UNWIND` clause node. */
extern const cypher_astnode_type_t CYPHER_AST_UNWIND;
/** Type for an AST `CALL` clause node. */
extern const cypher_astnode_type_t CYPHER_AST_CALL;
/** Type for an AST `RETURN` clause node. */
extern const cypher_astnode_type_t CYPHER_AST_RETURN;
/** Type for an AST projection node. */
extern const cypher_astnode_type_t CYPHER_AST_PROJECTION;
/** Type for an AST `ORDER BY` node. */
extern const cypher_astnode_type_t CYPHER_AST_ORDER_BY;
/** Type for an AST sort item node. */
extern const cypher_astnode_type_t CYPHER_AST_SORT_ITEM;
/** Type for an AST `UNION` clause node. */
extern const cypher_astnode_type_t CYPHER_AST_UNION;
/** Type for an AST expression node. */
extern const cypher_astnode_type_t CYPHER_AST_EXPRESSION;
/** Type for an AST unary operator node. */
extern const cypher_astnode_type_t CYPHER_AST_UNARY_OPERATOR;
/** Type for an AST binary operator node. */
extern const cypher_astnode_type_t CYPHER_AST_BINARY_OPERATOR;
/** Type for an AST n-ary comparison operator node. */
extern const cypher_astnode_type_t CYPHER_AST_COMPARISON;
/** Type for an AST apply operator node. */
extern const cypher_astnode_type_t CYPHER_AST_APPLY_OPERATOR;
/** Type for an AST apply * operator node. */
extern const cypher_astnode_type_t CYPHER_AST_APPLY_ALL_OPERATOR;
/** Type for an AST property operator node. */
extern const cypher_astnode_type_t CYPHER_AST_PROPERTY_OPERATOR;
/** Type for an AST subscript operator node. */
extern const cypher_astnode_type_t CYPHER_AST_SUBSCRIPT_OPERATOR;
/** Type for an AST subscript operator node. */
extern const cypher_astnode_type_t CYPHER_AST_SLICE_OPERATOR;
/** Type for an AST label check operator node. */
extern const cypher_astnode_type_t CYPHER_AST_LABELS_OPERATOR;
/** Type for an AST list comprehension node. */
extern const cypher_astnode_type_t CYPHER_AST_LIST_COMPREHENSION;
/** Type for an AST CASE expression node. */
extern const cypher_astnode_type_t CYPHER_AST_CASE;
/** Type for an AST filter expression node. */
extern const cypher_astnode_type_t CYPHER_AST_FILTER;
/** Type for an AST extract expression node. */
extern const cypher_astnode_type_t CYPHER_AST_EXTRACT;
/** Type for an AST reduce expression node. */
extern const cypher_astnode_type_t CYPHER_AST_REDUCE;
/** Type for an AST all predicate node. */
extern const cypher_astnode_type_t CYPHER_AST_ALL;
/** Type for an AST any predicate node. */
extern const cypher_astnode_type_t CYPHER_AST_ANY;
/** Type for an AST single predicate node. */
extern const cypher_astnode_type_t CYPHER_AST_SINGLE;
/** Type for an AST none predicate node. */
extern const cypher_astnode_type_t CYPHER_AST_NONE;
/** Type for an AST collection node. */
extern const cypher_astnode_type_t CYPHER_AST_COLLECTION;
/** Type for an AST literal map node. */
extern const cypher_astnode_type_t CYPHER_AST_MAP;
/** Type for an AST identifier node. */
extern const cypher_astnode_type_t CYPHER_AST_IDENTIFIER;
/** Type for an AST parameter node. */
extern const cypher_astnode_type_t CYPHER_AST_PARAMETER;
/** Type for an AST string literal node. */
extern const cypher_astnode_type_t CYPHER_AST_STRING;
/** Type for an AST integer literal node. */
extern const cypher_astnode_type_t CYPHER_AST_INTEGER;
/** Type for an AST float literal node. */
extern const cypher_astnode_type_t CYPHER_AST_FLOAT;
/** Type for an AST boolean literal node. */
extern const cypher_astnode_type_t CYPHER_AST_BOOLEAN;
/** Type for an AST TRUE literal node. */
extern const cypher_astnode_type_t CYPHER_AST_TRUE;
/** Type for an AST FALSE literal node. */
extern const cypher_astnode_type_t CYPHER_AST_FALSE;
/** Type for an AST NULL literal node. */
extern const cypher_astnode_type_t CYPHER_AST_NULL;
/** Type for an AST label node. */
extern const cypher_astnode_type_t CYPHER_AST_LABEL;
/** Type for an AST reltype node. */
extern const cypher_astnode_type_t CYPHER_AST_RELTYPE;
/** Type for an AST prop name node. */
extern const cypher_astnode_type_t CYPHER_AST_PROP_NAME;
/** Type for an AST function name node. */
extern const cypher_astnode_type_t CYPHER_AST_FUNCTION_NAME;
/** Type for an AST index name node. */
extern const cypher_astnode_type_t CYPHER_AST_INDEX_NAME;
/** Type for an AST procedure name node. */
extern const cypher_astnode_type_t CYPHER_AST_PROC_NAME;
/** Type for an AST pattern node. */
extern const cypher_astnode_type_t CYPHER_AST_PATTERN;
/** Type for an AST named pattern path node. */
extern const cypher_astnode_type_t CYPHER_AST_NAMED_PATH;
/** Type for an AST shortestPath node. */
extern const cypher_astnode_type_t CYPHER_AST_SHORTEST_PATH;
/** Type for an AST pattern path node. */
extern const cypher_astnode_type_t CYPHER_AST_PATTERN_PATH;
/** Type for an AST node pattern node. */
extern const cypher_astnode_type_t CYPHER_AST_NODE_PATTERN;
/** Type for an AST relationship pattern node. */
extern const cypher_astnode_type_t CYPHER_AST_REL_PATTERN;
/** Type for an AST range node. */
extern const cypher_astnode_type_t CYPHER_AST_RANGE;
/** Type for an AST command node. */
extern const cypher_astnode_type_t CYPHER_AST_COMMAND;
/** Type for an AST comment node. */
extern const cypher_astnode_type_t CYPHER_AST_COMMENT;
/** Type for an AST line comment node. */
extern const cypher_astnode_type_t CYPHER_AST_LINE_COMMENT;
/** Type for an AST block comment node. */
extern const cypher_astnode_type_t CYPHER_AST_BLOCK_COMMENT;
/** Type for an AST error node. */
extern const cypher_astnode_type_t CYPHER_AST_ERROR;

/**
 * A cypher expression operator.
 */
typedef struct cypher_operator cypher_operator_t;

/** The infix binary `OR` operator. */
extern const cypher_operator_t *CYPHER_OP_OR;
/** The infix binary `XOR` operator. */
extern const cypher_operator_t *CYPHER_OP_XOR;
/** The infix binary `AND` operator. */
extern const cypher_operator_t *CYPHER_OP_AND;
/** The prefix unary `NOT` operator. */
extern const cypher_operator_t *CYPHER_OP_NOT;
/** The infix binary `=` operator. */
extern const cypher_operator_t *CYPHER_OP_EQUAL;
/** The infix binary `<>` operator. */
extern const cypher_operator_t *CYPHER_OP_NEQUAL;
/** The n-ary `<` operator. */
extern const cypher_operator_t *CYPHER_OP_LT;
/** The n-ary `>` operator. */
extern const cypher_operator_t *CYPHER_OP_GT;
/** The n-ary `<=` operator. */
extern const cypher_operator_t *CYPHER_OP_LTE;
/** The n-ary `>=` operator. */
extern const cypher_operator_t *CYPHER_OP_GTE;
/** The infix binary `+` operator. */
extern const cypher_operator_t *CYPHER_OP_PLUS;
/** The infix binary `-` operator. */
extern const cypher_operator_t *CYPHER_OP_MINUS;
/** The infix binary `*` operator. */
extern const cypher_operator_t *CYPHER_OP_MULT;
/** The infix binary `/` operator. */
extern const cypher_operator_t *CYPHER_OP_DIV;
/** The infix binary `%` operator. */
extern const cypher_operator_t *CYPHER_OP_MOD;
/** The infix binary `^` operator. */
extern const cypher_operator_t *CYPHER_OP_POW;
/** The prefix unary `+` operator. */
extern const cypher_operator_t *CYPHER_OP_UNARY_PLUS;
/** The prefix unary `-` operator. */
extern const cypher_operator_t *CYPHER_OP_UNARY_MINUS;
/** The postfix binary subscript (`[exp]`) operator. */
extern const cypher_operator_t *CYPHER_OP_SUBSCRIPT;
/** The infix binary `=~` operator. */
extern const cypher_operator_t *CYPHER_OP_REGEX;
/** The infix binary `IN` operator. */
extern const cypher_operator_t *CYPHER_OP_IN;
/** The infix binary `STARTS WITH` operator. */
extern const cypher_operator_t *CYPHER_OP_STARTS_WITH;
/** The infix binary `CONTAINS` operator. */
extern const cypher_operator_t *CYPHER_OP_CONTAINS;
/** The infix binary `IS NULL` operator. */
extern const cypher_operator_t *CYPHER_OP_IS_NULL;
/** The infix binary `IS NOT NULL` operator. */
extern const cypher_operator_t *CYPHER_OP_IS_NOT_NULL;
/** The infix binary property lookup (`l.r`) operator. */
extern const cypher_operator_t *CYPHER_OP_PROPERTY;
/** The n-ary label check (`l:r`) operator. */
extern const cypher_operator_t *CYPHER_OP_LABEL;

/**
 * Get the type of an AST node.
 *
 * @param [node] The AST node.
 * @return The type of the node.
 */
__cypherlang_pure
cypher_astnode_type_t cypher_astnode_type(const cypher_astnode_t *node);

/**
 * Check the type of an AST node.
 *
 * @param [node] The AST node.
 * @param [type] The AST node type.
 * @return `true` if the node is of the specified type and `false` otherwise.
 */
__cypherlang_pure
bool cypher_astnode_instanceof(const cypher_astnode_t *node,
        cypher_astnode_type_t type);

/**
 * Get a string description of the AST node type.
 *
 * @param [type] The AST node type.
 * @return A pointer to a null terminated string containing the type name.
 */
__cypherlang_pure
const char *cypher_astnode_typestr(cypher_astnode_type_t type);

/**
 * Get the number of children from an AST node.
 *
 * @param [node] The AST node.
 * @return The number of children.
 */
__cypherlang_pure
unsigned int cypher_astnode_nchildren(const cypher_astnode_t *node);

/**
 * Get a child from an AST node.
 *
 * @param [node] The AST node.
 * @param [index] The index of the child.
 * @return A pointer to the child of the AST node, or `NULL` if there is no
 *         argument at the specified index.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_astnode_get_child(const cypher_astnode_t *node,
        unsigned int index);

/**
 * A position in the input.
 */
struct cypher_input_position
{
    unsigned int line;
    unsigned int column;
    size_t offset;
};

extern const struct cypher_input_position cypher_input_position_zero;

/**
 * A range in the input.
 */
struct cypher_input_range
{
    struct cypher_input_position start;
    struct cypher_input_position end;
};

/**
 * Get the range of an AST node in the input.
 *
 * @param [node] The AST node.
 * @return The range.
 */
__cypherlang_pure
struct cypher_input_range cypher_astnode_range(const cypher_astnode_t *node);

/**
 * The direction of a relationship pattern.
 */
enum cypher_rel_direction
{
    CYPHER_REL_INBOUND,
    CYPHER_REL_OUTBOUND,
    CYPHER_REL_BIDIRECTIONAL
};


/**
 * Construct a `CYPHER_AST_STATEMENT` node.
 *
 * @param [options] Options for the statement, all of type
 *         `CYPHER_AST_STATEMENT_OPTION`.
 * @param [noptions] The number of options (may be zero).
 * @param [body] The body of the statement, which must be either an
 *         `CYPHER_AST_QUERY` or `CYPHER_AST_SCHEMA_COMMAND`.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_statement(cypher_astnode_t * const *options,
        unsigned int noptions, const cypher_astnode_t *body,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the number of options in a `CYPHER_AST_STATEMENT` node.
 *
 * If the node is not an instance of `CYPHER_AST_STATEMENT` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return The number of options.
 */
__cypherlang_pure
unsigned int cypher_ast_statement_noptions(const cypher_astnode_t *node);

/**
 * Get an option of a `CYPHER_AST_STATEMENT` node.
 *
 * If the node is not an instance of `CYPHER_AST_STATEMENT` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the option.
 * @return A `CYPHER_AST_STATEMENT_OPTION` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_statement_get_option(
        const cypher_astnode_t *node, unsigned int index);

/**
 * Get the body of a `CYPHER_AST_STATEMENT` node.
 *
 * If the node is not an instance of `CYPHER_AST_STATEMENT` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_QUERY` or `CYPHER_AST_SCHEMA_COMMAND` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_statement_get_body(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_CYPHER_OPTION` node.
 *
 * The node will also be an instance of `CYPHER_AST_STATEMENT_OPTION`.
 *
 * @param [version] A version node of type `CYPHER_AST_STRING`, or
 *         null.
 * @param [params] Parameters for the option, all of type
 *         `CYPHER_AST_CYPHER_OPTION_PARAM`.
 * @param [nparams] The number of parameters (maybe zero).
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_cypher_option(const cypher_astnode_t *version,
        cypher_astnode_t * const *params, unsigned int nparams,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the version of a `CYPHER_AST_CYPHER_OPTION` node.
 *
 * If the node is not an instance of `CYPHER_AST_CYPHER_OPTION` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_STRING` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_cypher_option_get_version(
        const cypher_astnode_t *node);

/**
 * Get the number of parameters in a `CYPHER_AST_CYPHER_OPTION` node.
 *
 * If the node is not an instance of `CYPHER_AST_CYPHER_OPTION` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return The number of parameters.
 */
__cypherlang_pure
unsigned int cypher_ast_cypher_option_nparams(const cypher_astnode_t *node);

/**
 * Get an option of a `CYPHER_AST_CYPHER_OPTION` node.
 *
 * If the node is not an instance of `CYPHER_AST_CYPHER_OPTION` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the parameter.
 * @return A `CYPHER_AST_CYPHER_OPTION_PARAM` node, or null if no parameter
 *         is at the specified index.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_cypher_option_get_param(
        const cypher_astnode_t *node, unsigned int index);


/**
 * Construct a `CYPHER_AST_CYPHER_OPTION_PARAM` node.
 *
 * @param [name] A parameter name node, of type `CYPHER_AST_STRING`.
 * @param [value] A parameter value node, of type `CYPHER_AST_STRING`.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_cypher_option_param(const cypher_astnode_t *name,
        const cypher_astnode_t *value, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get an name of a `CYPHER_AST_CYPHER_OPTION_PARAM` node.
 *
 * If the node is not an instance of `CYPHER_AST_CYPHER_OPTION_PARAM` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_STRING` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_cypher_option_param_get_name(
        const cypher_astnode_t *node);

/**
 * Get an value of a `CYPHER_AST_CYPHER_OPTION_PARAM` node.
 *
 * If the node is not an instance of `CYPHER_AST_CYPHER_OPTION_PARAM` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_STRING` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_cypher_option_param_get_value(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_EXPLAIN_OPTION` node.
 *
 * The node will also be an instance of `CYPHER_AST_STATEMENT_OPTION`.
 *
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_explain_option(cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);


/**
 * Construct a `CYPHER_AST_PROFILE_OPTION` node.
 *
 * The node will also be an instance of `CYPHER_AST_STATEMENT_OPTION`.
 *
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_profile_option(cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);


/**
 * Construct a `CYPHER_AST_CREATE_NODE_PROP_INDEX` node.
 *
 * The node will also be an instance of `CYPHER_AST_SCHEMA_COMMAND`.
 *
 * @param [label] A label node, of type `CYPHER_AST_LABEL`.
 * @param [prop_name] A property name node, of type `CYPHER_AST_PROP_NAME`.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_create_node_prop_index(
        const cypher_astnode_t *label, const cypher_astnode_t *prop_name,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the label of a `CYPHER_AST_CREATE_NODE_PROP_INDEX` node.
 *
 * If the node is not an instance of `CYPHER_AST_CREATE_NODE_PROP_INDEX` then
 * the result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_LABEL` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_create_node_prop_index_get_label(
        const cypher_astnode_t *node);

/**
 * Get the property name of a `CYPHER_AST_CREATE_NODE_PROP_INDEX` node.
 *
 * If the node is not an instance of `CYPHER_AST_CREATE_NODE_PROP_INDEX` then
 * the result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_PROP_NAME` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_create_node_prop_index_get_prop_name(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_DROP_NODE_PROP_INDEX` node.
 *
 * The node will also be an instance of `CYPHER_AST_SCHEMA_COMMAND`.
 *
 * @param [label] A label node, of type `CYPHER_AST_LABEL`.
 * @param [prop_name] A property name node, of type `CYPHER_AST_PROP_NAME`.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_drop_node_prop_index(const cypher_astnode_t *label,
        const cypher_astnode_t *prop_name, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the label of a `CYPHER_AST_DROP_NODE_PROP_INDEX` node.
 *
 * If the node is not an instance of `CYPHER_AST_DROP_NODE_PROP_INDEX` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_LABEL` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_drop_node_prop_index_get_label(
        const cypher_astnode_t *node);

/**
 * Get the property name of a `CYPHER_AST_DROP_NODE_PROP_INDEX` node.
 *
 * If the node is not an instance of `CYPHER_AST_DROP_NODE_PROP_INDEX` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_PROP_NAME` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_drop_node_prop_index_get_prop_name(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_CREATE_NODE_PROP_CONSTRAINT` node.
 *
 * The node will also be an instance of `CYPHER_AST_SCHEMA_COMMAND`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [label] A `CYPHER_AST_LABEL` node.
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [unique] `true` if the constraint is unique, and `false` otherwise.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_create_node_prop_constraint(
        const cypher_astnode_t *identifier, const cypher_astnode_t *label,
        const cypher_astnode_t *expression, bool unique,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the identifier of a `CYPHER_AST_CREATE_NODE_PROP_CONSTRAINT` node.
 *
 * If the node is not an instance of `CYPHER_AST_CREATE_NODE_PROP_CONSTRAINT`
 * then the result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_create_node_prop_constraint_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the label of a `CYPHER_AST_CREATE_NODE_PROP_CONSTRAINT` node.
 *
 * If the node is not an instance of `CYPHER_AST_CREATE_NODE_PROP_CONSTRAINT`
 * then the result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_LABEL` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_create_node_prop_constraint_get_label(
        const cypher_astnode_t *node);

/**
 * Get the expression of a `CYPHER_AST_CREATE_NODE_PROP_CONSTRAINT` node.
 *
 * If the node is not an instance of `CYPHER_AST_CREATE_NODE_PROP_CONSTRAINT`
 * then the result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_create_node_prop_constraint_get_expression(
        const cypher_astnode_t *node);

/**
 * Check if the constraint of a `CYPHER_AST_CREATE_NODE_PROP_CONSTRAINT` node is unique.
 *
 * If the node is not an instance of `CYPHER_AST_CREATE_NODE_PROP_CONSTRAINT`
 * then the result will be undefined.
 *
 * @param [node] The AST node.
 * @return `true` if the constraint is unique, and `false` otherwise.
 */
bool cypher_ast_create_node_prop_constraint_is_unique(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_DROP_NODE_PROP_CONSTRAINT` node.
 *
 * The node will also be an instance of `CYPHER_AST_SCHEMA_COMMAND`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [label] A `CYPHER_AST_LABEL` node.
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [unique] `true` if the constraint is unique, and `false` otherwise.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_drop_node_prop_constraint(
        const cypher_astnode_t *identifier, const cypher_astnode_t *label,
        const cypher_astnode_t *expression, bool unique,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the identifier of a `CYPHER_AST_DROP_NODE_PROP_CONSTRAINT` node.
 *
 * If the node is not an instance of `CYPHER_AST_DROP_NODE_PROP_CONSTRAINT`
 * then the result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_drop_node_prop_constraint_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the label of a `CYPHER_AST_DROP_NODE_PROP_CONSTRAINT` node.
 *
 * If the node is not an instance of `CYPHER_AST_DROP_NODE_PROP_CONSTRAINT`
 * then the result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_LABEL` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_drop_node_prop_constraint_get_label(
        const cypher_astnode_t *node);

/**
 * Get the expression of a `CYPHER_AST_DROP_NODE_PROP_CONSTRAINT` node.
 *
 * If the node is not an instance of `CYPHER_AST_DROP_NODE_PROP_CONSTRAINT`
 * then the result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_drop_node_prop_constraint_get_expression(
        const cypher_astnode_t *node);

/**
 * Check if the constraint of a `CYPHER_AST_DROP_NODE_PROP_CONSTRAINT` node is unique.
 *
 * If the node is not an instance of `CYPHER_AST_DROP_NODE_PROP_CONSTRAINT`
 * then the result will be undefined.
 *
 * @param [node] The AST node.
 * @return `true` if the constraint is unique, and `false` otherwise.
 */
bool cypher_ast_drop_node_prop_constraint_is_unique(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_CREATE_REL_PROP_CONSTRAINT` node.
 *
 * The node will also be an instance of `CYPHER_AST_SCHEMA_COMMAND`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [label] A `CYPHER_AST_LABEL` node.
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [unique] `true` if the constraint is unique, and `false` otherwise.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_create_rel_prop_constraint(
        const cypher_astnode_t *identifier, const cypher_astnode_t *label,
        const cypher_astnode_t *expression, bool unique,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the identifier of a `CYPHER_AST_CREATE_REL_PROP_CONSTRAINT` node.
 *
 * If the node is not an instance of `CYPHER_AST_CREATE_REL_PROP_CONSTRAINT`
 * then the result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_create_rel_prop_constraint_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the relationship type of a `CYPHER_AST_CREATE_REL_PROP_CONSTRAINT` node.
 *
 * If the node is not an instance of `CYPHER_AST_CREATE_REL_PROP_CONSTRAINT`
 * then the result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_RELTYPE` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_create_rel_prop_constraint_get_reltype(
        const cypher_astnode_t *node);

/**
 * Get the expression of a `CYPHER_AST_CREATE_REL_PROP_CONSTRAINT` node.
 *
 * If the node is not an instance of `CYPHER_AST_CREATE_REL_PROP_CONSTRAINT`
 * then the result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_create_rel_prop_constraint_get_expression(
        const cypher_astnode_t *node);

/**
 * Check if the constraint of a `CYPHER_AST_CREATE_REL_PROP_CONSTRAINT` node is unique.
 *
 * If the node is not an instance of `CYPHER_AST_CREATE_REL_PROP_CONSTRAINT`
 * then the result will be undefined.
 *
 * @param [node] The AST node.
 * @return `true` if the constraint is unique, and `false` otherwise.
 */
bool cypher_ast_create_rel_prop_constraint_is_unique(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_DROP_REL_PROP_CONSTRAINT` node.
 *
 * The node will also be an instance of `CYPHER_AST_SCHEMA_COMMAND`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [label] A `CYPHER_AST_LABEL` node.
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [unique] `true` if the constraint is unique, and `false` otherwise.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_drop_rel_prop_constraint(
        const cypher_astnode_t *identifier, const cypher_astnode_t *label,
        const cypher_astnode_t *expression, bool unique,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the identifier of a `CYPHER_AST_DROP_REL_PROP_CONSTRAINT` node.
 *
 * If the node is not an instance of `CYPHER_AST_DROP_REL_PROP_CONSTRAINT`
 * then the result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_drop_rel_prop_constraint_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the relationship type of a `CYPHER_AST_DROP_REL_PROP_CONSTRAINT` node.
 *
 * If the node is not an instance of `CYPHER_AST_DROP_REL_PROP_CONSTRAINT`
 * then the result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_RELTYPE` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_drop_rel_prop_constraint_get_reltype(
        const cypher_astnode_t *node);

/**
 * Get the expression of a `CYPHER_AST_DROP_REL_PROP_CONSTRAINT` node.
 *
 * If the node is not an instance of `CYPHER_AST_DROP_REL_PROP_CONSTRAINT`
 * then the result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_drop_rel_prop_constraint_get_expression(
        const cypher_astnode_t *node);

/**
 * Check if the constraint of a `CYPHER_AST_DROP_REL_PROP_CONSTRAINT` node is unique.
 *
 * If the node is not an instance of `CYPHER_AST_DROP_REL_PROP_CONSTRAINT`
 * then the result will be undefined.
 *
 * @param [node] The AST node.
 * @return `true` if the constraint is unique, and `false` otherwise.
 */
bool cypher_ast_drop_rel_prop_constraint_is_unique(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_QUERY` node.
 *
 * @param [options] Options for the query, all of type
 *         `CYPHER_AST_QUERY_OPTION`.
 * @param [noptions] The number of options (may be zero).
 * @param [clauses] Clauses for the query, all of type
 *         `CYPHER_AST_QUERY_CLAUSE`.
 * @param [nclauses] The number of clauses.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_query(cypher_astnode_t * const *options,
        unsigned int noptions, cypher_astnode_t * const *clauses,
        unsigned int nclauses, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the number of options in a `CYPHER_AST_QUERY` node.
 *
 * If the node is not an instance of `CYPHER_AST_QUERY` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return The number of options.
 */
__cypherlang_pure
unsigned int cypher_ast_query_noptions(const cypher_astnode_t *node);

/**
 * Get an option of a `CYPHER_AST_QUERY` node.
 *
 * If the node is not an instance of `CYPHER_AST_QUERY` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the option.
 * @return A `CYPHER_AST_QUERY_OPTION` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_query_get_option(
        const cypher_astnode_t *node, unsigned int index);

/**
 * Get the number of clauses in a `CYPHER_AST_QUERY` node.
 *
 * If the node is not an instance of `CYPHER_AST_QUERY` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return The number of clauses.
 */
__cypherlang_pure
unsigned int cypher_ast_query_nclauses(const cypher_astnode_t *node);

/**
 * Get a clause of a `CYPHER_AST_QUERY` node.
 *
 * If the node is not an instance of `CYPHER_AST_QUERY` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the clause.
 * @return A `CYPHER_AST_QUERY_OPTION` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_query_get_clause(
        const cypher_astnode_t *node, unsigned int index);


/**
 * Construct a `CYPHER_AST_USING_PERIODIC_COMMIT` node.
 *
 * The node will also be an instance of `CYPHER_AST_QUERY_OPTION`.
 *
 * @param [limit] A `CYPHER_AST_INTEGER`, or null.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_using_periodic_commit(
        const cypher_astnode_t *limit, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the limit of a `CYPHER_AST_USING_PERIODIC_COMMIT` node.
 *
 * If the node is not an instance of `CYPHER_AST_USING_PERIODIC_COMMIT` then
 * the result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_INTEGER` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_using_periodic_commit_get_limit(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_LOAD_CSV` node.
 *
 * The node will also be an instance of `CYPHER_AST_QUERY_CLAUSE`.
 *
 * @param [with_headers] `true` if the `WITH HEADERS` option is enabled, and
 *         `false` otherwise.
 * @param [url] A `CYPHER_AST_EXPRESSION` node.
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [field_terminator] A `CYPHER_AST_STRING` node, or null.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_load_csv(bool with_headers,
        const cypher_astnode_t *url, const cypher_astnode_t *identifier,
        const cypher_astnode_t *field_terminator, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Check the with_headers value of a `CYPHER_AST_LOAD_CSV` node.
 *
 * If the node is not an instance of `CYPHER_AST_LOAD_CSV` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return `true` if the `WITH HEADERS` option is enabled,
 *         and `false` otherwise.
 */
__cypherlang_pure
bool cypher_ast_load_csv_has_with_headers(const cypher_astnode_t *node);

/**
 * Get the url of a `CYPHER_AST_LOAD_CSV` node.
 *
 * If the node is not an instance of `CYPHER_AST_LOAD_CSV` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_load_csv_get_url(
        const cypher_astnode_t *node);

/**
 * Get the identifier of a `CYPHER_AST_LOAD_CSV` node.
 *
 * If the node is not an instance of `CYPHER_AST_LOAD_CSV` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_load_csv_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the field terminator of a `CYPHER_AST_LOAD_CSV` node.
 *
 * If the node is not an instance of `CYPHER_AST_LOAD_CSV` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_STRING` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_load_csv_get_field_terminator(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_START` node.
 *
 * The node will also be an instance of `CYPHER_AST_QUERY_CLAUSE`.
 *
 * @param [points] Start points for the statement, all of type
 *         `CYPHER_AST_START_POINT`.
 * @param [npoints] The number of start points.
 * @param [predicate] A `CYPHER_AST_EXPRESSION`, or null.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_start(cypher_astnode_t * const *points,
        unsigned int npoints, const cypher_astnode_t *predicate,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the number of start points in a `CYPHER_AST_START` node.
 *
 * If the node is not an instance of `CYPHER_AST_START` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return The number of start points.
 */
__cypherlang_pure
unsigned int cypher_ast_start_npoints(const cypher_astnode_t *node);

/**
 * Get a start point of a `CYPHER_AST_START` node.
 *
 * If the node is not an instance of `CYPHER_AST_START` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the start point.
 * @return A `CYPHER_AST_QUERY_OPTION` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_start_get_point(
        const cypher_astnode_t *node, unsigned int index);

/**
 * Get the predicate of a `CYPHER_AST_START` node.
 *
 * If the node is not an instance of `CYPHER_AST_START` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_start_get_predicate(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_NODE_INDEX_LOOKUP` node.
 *
 * The node will also be an instance of `CYPHER_AST_START_POINT`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [index_name] A `CYPHER_AST_INDEX_NAME` node.
 * @param [prop_name] A `CYPHER_AST_PROP_NAME` node.
 * @param [lookup] A `CYPHER_AST_STRING` or `CYPHER_AST_PARAMETER` node.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_node_index_lookup(
        const cypher_astnode_t *identifier, const cypher_astnode_t *index_name,
        const cypher_astnode_t *prop_name, const cypher_astnode_t *lookup,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the identifier of a `CYPHER_AST_NODE_INDEX_LOOKUP` node.
 *
 * If the node is not an instance of `CYPHER_AST_NODE_INDEX_LOOKUP` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_node_index_lookup_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the index name of a `CYPHER_AST_NODE_INDEX_LOOKUP` node.
 *
 * If the node is not an instance of `CYPHER_AST_NODE_INDEX_LOOKUP` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_INDEX_NAME` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_node_index_lookup_get_index_name(
        const cypher_astnode_t *node);

/**
 * Get the prop name of a `CYPHER_AST_NODE_INDEX_LOOKUP` node.
 *
 * If the node is not an instance of `CYPHER_AST_NODE_INDEX_LOOKUP` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_PROP_NAME` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_node_index_lookup_get_prop_name(
        const cypher_astnode_t *node);

/**
 * Get the lookup of a `CYPHER_AST_NODE_INDEX_LOOKUP` node.
 *
 * If the node is not an instance of `CYPHER_AST_NODE_INDEX_LOOKUP` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_STRING` or `CYPHER_AST_PARAMETER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_node_index_lookup_get_lookup(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_NODE_INDEX_QUERY` node.
 *
 * The node will also be an instance of `CYPHER_AST_START_POINT`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [index_name] A `CYPHER_AST_INDEX_NAME` node.
 * @param [query] A `CYPHER_AST_STRING` or `CYPHER_AST_PARAMETER` node.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_node_index_query(
        const cypher_astnode_t *identifier, const cypher_astnode_t *index_name,
        const cypher_astnode_t *query, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the identifier of a `CYPHER_AST_NODE_INDEX_QUERY` node.
 *
 * If the node is not an instance of `CYPHER_AST_NODE_INDEX_QUERY` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_node_index_query_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the index name of a `CYPHER_AST_NODE_INDEX_QUERY` node.
 *
 * If the node is not an instance of `CYPHER_AST_NODE_INDEX_QUERY` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_INDEX_NAME` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_node_index_query_get_index_name(
        const cypher_astnode_t *node);

/**
 * Get the query of a `CYPHER_AST_NODE_INDEX_QUERY` node.
 *
 * If the node is not an instance of `CYPHER_AST_NODE_INDEX_QUERY` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_STRING` or `CYPHER_AST_PARAMETER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_node_index_query_get_query(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_NODE_ID_LOOKUP` node.
 *
 * The node will also be an instance of `CYPHER_AST_START_POINT`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [ids] Node IDs for lookup, all of type `CYPHER_AST_INTEGER`.
 * @param [nids] The number of node IDs.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_node_id_lookup(const cypher_astnode_t *identifier,
        cypher_astnode_t * const *ids, unsigned int nids,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the identifier of a `CYPHER_AST_NODE_ID_LOOKUP` node.
 *
 * If the node is not an instance of `CYPHER_AST_NODE_ID_LOOKUP` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_node_id_lookup_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the number of ids in a `CYPHER_AST_NODE_ID_LOOKUP` node.
 *
 * If the node is not an instance of `CYPHER_AST_NODE_ID_LOOKUP` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return The number of ids.
 */
__cypherlang_pure
unsigned int cypher_ast_node_id_lookup_nids(const cypher_astnode_t *node);

/**
 * Get an id of a `CYPHER_AST_NODE_ID_LOOKUP` node.
 *
 * If the node is not an instance of `CYPHER_AST_NODE_ID_LOOKUP` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the id.
 * @return A `CYPHER_AST_INTEGER` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_node_id_lookup_get_id(
        const cypher_astnode_t *node, unsigned int index);


/**
 * Construct a `CYPHER_AST_ALL_NODES_SCAN` node.
 *
 * The node will also be an instance of `CYPHER_AST_START_POINT`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_all_nodes_scan(const cypher_astnode_t *identifier,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the identifier of a `CYPHER_AST_ALL_NODES_SCAN` node.
 *
 * If the node is not an instance of `CYPHER_AST_ALL_NODES_SCAN` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_all_nodes_scan_get_identifier(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_REL_INDEX_LOOKUP` node.
 *
 * The node will also be an instance of `CYPHER_AST_START_POINT`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [index_name] A `CYPHER_AST_INDEX_NAME` node.
 * @param [prop_name] A `CYPHER_AST_PROP_NAME` node.
 * @param [lookup] A `CYPHER_AST_STRING` or `CYPHER_AST_PARAMETER` node.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_rel_index_lookup(
        const cypher_astnode_t *identifier, const cypher_astnode_t *index_name,
        const cypher_astnode_t *prop_name, const cypher_astnode_t *lookup,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);


/**
 * Get the identifier of a `CYPHER_AST_REL_INDEX_LOOKUP` node.
 *
 * If the node is not an instance of `CYPHER_AST_REL_INDEX_LOOKUP` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_rel_index_lookup_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the index name of a `CYPHER_AST_REL_INDEX_LOOKUP` node.
 *
 * If the node is not an instance of `CYPHER_AST_REL_INDEX_LOOKUP` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_INDEX_NAME` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_rel_index_lookup_get_index_name(
        const cypher_astnode_t *node);

/**
 * Get the prop name of a `CYPHER_AST_REL_INDEX_LOOKUP` node.
 *
 * If the node is not an instance of `CYPHER_AST_REL_INDEX_LOOKUP` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_PROP_NAME` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_rel_index_lookup_get_prop_name(
        const cypher_astnode_t *node);

/**
 * Get the lookup of a `CYPHER_AST_REL_INDEX_LOOKUP` node.
 *
 * If the node is not an instance of `CYPHER_AST_REL_INDEX_LOOKUP` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_STRING` or `CYPHER_AST_PARAMETER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_rel_index_lookup_get_lookup(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_REL_INDEX_QUERY` node.
 *
 * The node will also be an instance of `CYPHER_AST_START_POINT`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [index_name] A `CYPHER_AST_INDEX_NAME` node.
 * @param [query] A `CYPHER_AST_STRING` or `CYPHER_AST_PARAMETER` node.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_rel_index_query(
        const cypher_astnode_t *identifier, const cypher_astnode_t *index_name,
        const cypher_astnode_t *query, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the identifier of a `CYPHER_AST_REL_INDEX_QUERY` node.
 *
 * If the node is not an instance of `CYPHER_AST_REL_INDEX_QUERY` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_rel_index_query_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the index name of a `CYPHER_AST_REL_INDEX_QUERY` node.
 *
 * If the node is not an instance of `CYPHER_AST_REL_INDEX_QUERY` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_INDEX_NAME` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_rel_index_query_get_index_name(
        const cypher_astnode_t *node);

/**
 * Get the query of a `CYPHER_AST_REL_INDEX_QUERY` node.
 *
 * If the node is not an instance of `CYPHER_AST_REL_INDEX_QUERY` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_STRING` or `CYPHER_AST_PARAMETER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_rel_index_query_get_query(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_REL_ID_LOOKUP` node.
 *
 * The node will also be an instance of `CYPHER_AST_START_POINT`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [ids] Node IDs for lookup, all of type `CYPHER_AST_INTEGER`.
 * @param [nids] The number of node IDs.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_rel_id_lookup(const cypher_astnode_t *identifier,
        cypher_astnode_t * const *ids, unsigned int nids,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);


/**
 * Get the identifier of a `CYPHER_AST_REL_ID_LOOKUP` node.
 *
 * If the node is not an instance of `CYPHER_AST_REL_ID_LOOKUP` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_rel_id_lookup_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the number of ids in a `CYPHER_AST_REL_ID_LOOKUP` node.
 *
 * If the node is not an instance of `CYPHER_AST_REL_ID_LOOKUP` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return The number of ids.
 */
__cypherlang_pure
unsigned int cypher_ast_rel_id_lookup_nids(const cypher_astnode_t *node);

/**
 * Get an id of a `CYPHER_AST_REL_ID_LOOKUP` node.
 *
 * If the node is not an instance of `CYPHER_AST_REL_ID_LOOKUP` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the id.
 * @return A `CYPHER_AST_INTEGER` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_rel_id_lookup_get_id(
        const cypher_astnode_t *node, unsigned int index);


/**
 * Construct a `CYPHER_AST_ALL_RELS_SCAN` node.
 *
 * The node will also be an instance of `CYPHER_AST_START_POINT`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_all_rels_scan(const cypher_astnode_t *identifier,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the identifier of a `CYPHER_AST_ALL_RELS_SCAN` node.
 *
 * If the node is not an instance of `CYPHER_AST_ALL_RELS_SCAN` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_all_rels_scan_get_identifier(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_MATCH` node.
 *
 * The node will also be an instance of `CYPHER_AST_CLAUSE`.
 *
 * @param [optional] `true` if the node represents an `OPTIONAL MATCH`, and
 *         `false` otherwise.
 * @param [pattern] A `CYPHER_AST_PATTERN` node.
 * @param [hints] Hints for lookup, all of type `CYPHER_AST_MATCH_HINT`.
 * @param [nhints] The number of match hints.
 * @param [predicate] A `CYPHER_AST_EXPRESSION` node, or null.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_match(bool optional,
        const cypher_astnode_t *pattern, cypher_astnode_t * const *hints,
        unsigned int nhints, const cypher_astnode_t *predicate,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Check if the optional value of a `CYPHER_AST_MATCH` node is set.
 *
 * If the node is not an instance of `CYPHER_AST_MATCH` then the result will be
 * undefined.
 *
 * @param [node] The AST node.
 * @return `true` if the `OPTIONAL MATCH` option is enabled,
 *         and `false` otherwise.
 */
__cypherlang_pure
bool cypher_ast_match_is_optional(const cypher_astnode_t *node);

/**
 * Get the pattern of a `CYPHER_AST_MATCH` node.
 *
 * If the node is not an instance of `CYPHER_AST_MATCH` then the result will be
 * undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_PATTERN` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_match_get_pattern(
        const cypher_astnode_t *node);

/**
 * Get the number of hints in a `CYPHER_AST_MATCH` node.
 *
 * If the node is not an instance of `CYPHER_AST_MATCH` then the result will be
 * undefined.
 *
 * @param [node] The AST node.
 * @return The number of hints.
 */
__cypherlang_pure
unsigned int cypher_ast_match_nhints(const cypher_astnode_t *node);

/**
 * Get a hint from a `CYPHER_AST_MATCH` node.
 *
 * If the node is not an instance of `CYPHER_AST_MATCH` then the result will be
 * undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the hint.
 * @return A `CYPHER_AST_MATCH_HINT` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_match_get_hint(
        const cypher_astnode_t *node, unsigned int index);

/**
 * Get the predicate of a `CYPHER_AST_PREDICATE` node.
 *
 * If the node is not an instance of `CYPHER_AST_PREDICATE` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_PREDICATE` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_match_get_predicate(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_USING_INDEX` node.
 *
 * The node will also be an instance of `CYPHER_AST_MATCH_HINT`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [label] A `CYPHER_AST_LABEL` node.
 * @param [prop_name] A `CYPHER_AST_PROP_NAME` node.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_using_index(const cypher_astnode_t *identifier,
        const cypher_astnode_t *label, const cypher_astnode_t *prop_name,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the identifer of a `CYPHER_AST_USING_INDEX` node.
 *
 * If the node is not an instance of `CYPHER_AST_USING_INDEX` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_using_index_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the label of a `CYPHER_AST_USING_INDEX` node.
 *
 * If the node is not an instance of `CYPHER_AST_USING_INDEX` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_using_index_get_label(
        const cypher_astnode_t *node);

/**
 * Get the label of a `CYPHER_AST_USING_INDEX` node.
 *
 * If the node is not an instance of `CYPHER_AST_USING_INDEX` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_PROP_NAME` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_using_index_get_prop_name(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_USING_JOIN` node.
 *
 * The node will also be an instance of `CYPHER_AST_MATCH_HINT`.
 *
 * @param [identifiers] Identifiers for the hint, all of type
 *         `CYPHER_AST_IDENTIFIER`.
 * @param [nidentifiers] The number of identifiers
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_using_join(
        cypher_astnode_t * const *identifiers, unsigned int nidentifiers,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the number of identifiers in a `CYPHER_AST_USING_JOIN` node.
 *
 * If the node is not an instance of `CYPHER_AST_USING_JOIN` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return The number of identifiers.
 */
__cypherlang_pure
unsigned int cypher_ast_using_join_nidentifiers(const cypher_astnode_t *node);

/**
 * Get a identifier from a `CYPHER_AST_USING_JOIN` node.
 *
 * If the node is not an instance of `CYPHER_AST_USING_JOIN` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the identifier.
 * @return A `CYPHER_AST_IDENTIFIER` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_using_join_get_identifier(
        const cypher_astnode_t *node, unsigned int index);


/**
 * Construct a `CYPHER_AST_USING_SCAN` node.
 *
 * The node will also be an instance of `CYPHER_AST_MATCH_HINT`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [label] A `CYPHER_AST_LABEL` node.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_using_scan(const cypher_astnode_t *identifier,
        const cypher_astnode_t *label, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the identifer of a `CYPHER_AST_USING_SCAN` node.
 *
 * If the node is not an instance of `CYPHER_AST_USING_SCAN` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_using_scan_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the label of a `CYPHER_AST_USING_SCAN` node.
 *
 * If the node is not an instance of `CYPHER_AST_USING_SCAN` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_using_scan_get_label(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_MERGE` node.
 *
 * The node will also be an instance of `CYPHER_AST_CLAUSE`.
 *
 * @param [path] A `CYPHER_AST_PATTERN_PATH` node.
 * @param [actions] Actions for merge, all of type `CYPHER_AST_MERGE_ACTION`.
 * @param [nactions] The number of match actions.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_merge(const cypher_astnode_t *path,
        cypher_astnode_t * const *actions, unsigned int nactions,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the pattern path of a `CYPHER_AST_MERGE` node.
 *
 * If the node is not an instance of `CYPHER_AST_MERGE` then the result will be
 * undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_PATTERN_PATH` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_merge_get_pattern_path(
        const cypher_astnode_t *node);

/**
 * Get the number of actions in a `CYPHER_AST_MERGE` node.
 *
 * If the node is not an instance of `CYPHER_AST_MERGE` then the result will be
 * undefined.
 *
 * @param [node] The AST node.
 * @return The number of actions.
 */
__cypherlang_pure
unsigned int cypher_ast_merge_nactions(const cypher_astnode_t *node);

/**
 * Get an action from a `CYPHER_AST_MERGE` node.
 *
 * If the node is not an instance of `CYPHER_AST_MERGE` then the result will be
 * undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the action.
 * @return A `CYPHER_AST_MERGE_HINT` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_merge_get_action(
        const cypher_astnode_t *node, unsigned int index);


/**
 * Construct a `CYPHER_AST_ON_MATCH` node.
 *
 * The node will also be an instance of `CYPHER_AST_MERGE_ACTION`.
 *
 * @param [items] Items for `ON MATCH`, all of type `CYPHER_AST_SET_ITEM`.
 * @param [nitems] The number of `ON MATCH` items.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_on_match(cypher_astnode_t * const *items,
        unsigned int nitems, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the number of items in a `CYPHER_AST_ON_MATCH` node.
 *
 * If the node is not an instance of `CYPHER_AST_ON_MATCH` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return The number of items.
 */
__cypherlang_pure
unsigned int cypher_ast_on_match_nitems(const cypher_astnode_t *node);

/**
 * Get an item from a `CYPHER_AST_ON_MATCH` node.
 *
 * If the node is not an instance of `CYPHER_AST_ON_MATCH` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the action.
 * @return A `CYPHER_AST_SET_ITEM` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_on_match_get_item(
        const cypher_astnode_t *node, unsigned int index);


/**
 * Construct a `CYPHER_AST_ON_CREATE` node.
 *
 * The node will also be an instance of `CYPHER_AST_MERGE_ACTION`.
 *
 * @param [items] Items for `ON CREATE`, all of type `CYPHER_AST_SET_ITEM`.
 * @param [nitems] The number of `ON CREATE` items.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_on_create(cypher_astnode_t * const *items,
        unsigned int nitems, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the number of items in a `CYPHER_AST_ON_CREATE` node.
 *
 * If the node is not an instance of `CYPHER_AST_ON_CREATE` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return The number of items.
 */
__cypherlang_pure
unsigned int cypher_ast_on_create_nitems(const cypher_astnode_t *node);

/**
 * Get an item from a `CYPHER_AST_ON_CREATE` node.
 *
 * If the node is not an instance of `CYPHER_AST_ON_CREATE` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the action.
 * @return A `CYPHER_AST_SET_ITEM` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_on_create_get_item(
        const cypher_astnode_t *node, unsigned int index);


/**
 * Construct a `CYPHER_AST_CREATE` node.
 *
 * The node will also be an instance of `CYPHER_AST_CLAUSE`.
 *
 * @param [unique] `true` if the create is unique, `false` otherwise.
 * @param [pattern] A `CYPHER_AST_PATTERN` node.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_create(bool unique,
        const cypher_astnode_t *pattern, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Check if the unique value of a `CYPHER_AST_CREATE` node is set.
 *
 * If the node is not an instance of `CYPHER_AST_CREATE` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return `true` if the `UNIQUE` option is enabled, and `false` otherwise.
 */
__cypherlang_pure
bool cypher_ast_create_is_unique(const cypher_astnode_t *node);

/**
 * Get the pattern of a `CYPHER_AST_CREATE` node.
 *
 * If the node is not an instance of `CYPHER_AST_CREATE` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_PATTERN` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_create_get_pattern(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_SET` node.
 *
 * The node will also be an instance of `CYPHER_AST_CLAUSE`.
 *
 * @param [items] Items for `SET`, all of type `CYPHER_AST_SET_ITEM`.
 * @param [nitems] The number of `SET` items.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_set(cypher_astnode_t * const *items,
        unsigned int nitems, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the number of items in a `CYPHER_AST_SET` node.
 *
 * If the node is not an instance of `CYPHER_AST_SET` then the result will be
 * undefined.
 *
 * @param [node] The AST node.
 * @return The number of items.
 */
__cypherlang_pure
unsigned int cypher_ast_set_nitems(const cypher_astnode_t *node);

/**
 * Get an item from a `CYPHER_AST_SET` node.
 *
 * If the node is not an instance of `CYPHER_AST_SET` then the result will be
 * undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the action.
 * @return A `CYPHER_AST_SET_ITEM` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_set_get_item(
        const cypher_astnode_t *node, unsigned int index);


/**
 * Construct a `CYPHER_AST_SET_PROPERTY` node.
 *
 * The node will also be an instance of `CYPHER_AST_SET_ITEM`.
 *
 * @param [property] A `CYPHER_AST_PROPERTY_OPERATOR` node.
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_set_property(const cypher_astnode_t *property,
        const cypher_astnode_t *expression, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the property operator of a `CYPHER_AST_SET_PROPERTY` node.
 *
 * If the node is not an instance of `CYPHER_AST_SET_PROPERTY` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_PROPERTY_OPERATOR` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_set_property_get_property(
        const cypher_astnode_t *node);

/**
 * Get the expression of a `CYPHER_AST_SET_PROPERTY` node.
 *
 * If the node is not an instance of `CYPHER_AST_SET_PROPERTY` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_set_property_get_expression(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_SET_ALL_PROPERTIES` node.
 *
 * The node will also be an instance of `CYPHER_AST_SET_ITEM`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_set_all_properties(
        const cypher_astnode_t *identifier, const cypher_astnode_t *expression,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the identifier of a `CYPHER_AST_SET_ALL_PROPERTIES` node.
 *
 * If the node is not an instance of `CYPHER_AST_SET_ALL_PROPERTIES` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_set_all_properties_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the expression of a `CYPHER_AST_SET_ALL_PROPERTIES` node.
 *
 * If the node is not an instance of `CYPHER_AST_SET_ALL_PROPERTIES` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_set_all_properties_get_expression(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_MERGE_PROPERTIES` node.
 *
 * The node will also be an instance of `CYPHER_AST_SET_ITEM`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_merge_properties(
        const cypher_astnode_t *identifier, const cypher_astnode_t *expression,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the identifier of a `CYPHER_AST_MERGE_PROPERTIES` node.
 *
 * If the node is not an instance of `CYPHER_AST_MERGE_PROPERTIES` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_merge_properties_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the expression of a `CYPHER_AST_MERGE_PROPERTIES` node.
 *
 * If the node is not an instance of `CYPHER_AST_MERGE_PROPERTIES` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_merge_properties_get_expression(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_SET_LABELS` node.
 *
 * The node will also be an instance of `CYPHER_AST_SET_ITEM`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [labels] The labels to be set, which must all be of type
 *         `CYPHER_AST_LABEL`.
 * @param [nlabels] The number of labels to be set.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_set_labels(const cypher_astnode_t *identifier,
        cypher_astnode_t * const *labels, unsigned int nlabels,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the identifier of a `CYPHER_AST_SET_LABELS` node.
 *
 * If the node is not an instance of `CYPHER_AST_SET_LABELS` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_set_labels_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the number of labels in a `CYPHER_AST_SET_LABELS` node.
 *
 * If the node is not an instance of `CYPHER_AST_SET_LABELS` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return The number of labels.
 */
__cypherlang_pure
unsigned int cypher_ast_set_labels_nlabels(const cypher_astnode_t *node);

/**
 * Get a label from a `CYPHER_AST_SET_LABELS` node.
 *
 * If the node is not an instance of `CYPHER_AST_SET_LABELS` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the label.
 * @return A `CYPHER_AST_LABEL` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_set_labels_get_label(
        const cypher_astnode_t *node, unsigned int index);


/**
 * Construct a `CYPHER_AST_DELETE` node.
 *
 * The node will also be an instance of `CYPHER_AST_CLAUSE`.
 *
 * @param [detach] `true` if the `DETACH` option is present, `false` otherwise.
 * @param [expressions] Expressions for `DELETE`, all of type
 *         `CYPHER_AST_EXPRESSION`.
 * @param [nexpressions] The number of expressions.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_delete(bool detach,
        cypher_astnode_t * const *expressions, unsigned int nexpressions,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Check if `DETACH` is present for a `CYPHER_AST_DELETE` node.
 *
 * If the node is not an instance of `CYPHER_AST_DELETE` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return `true` if `DETACH` is present, `false` otherwise.
 */
__cypherlang_pure
bool cypher_ast_delete_has_detach(const cypher_astnode_t *node);

/**
 * Get the number of expression in a `CYPHER_AST_DELETE` node.
 *
 * If the node is not an instance of `CYPHER_AST_DELETE` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return The number of expression.
 */
__cypherlang_pure
unsigned int cypher_ast_delete_nexpressions(const cypher_astnode_t *node);

/**
 * Get an expression from a `CYPHER_AST_DELETE` node.
 *
 * If the node is not an instance of `CYPHER_AST_DELETE` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the expression.
 * @return A `CYPHER_AST_EXPRESSION` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_delete_get_expression(
        const cypher_astnode_t *node, unsigned int index);


/**
 * Construct a `CYPHER_AST_REMOVE` node.
 *
 * The node will also be an instance of `CYPHER_AST_CLAUSE`.
 *
 * @param [items] Items for `REMOVE`, all of type `CYPHER_AST_REMOVE_ITEM`.
 * @param [nitems] The number of items.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_remove(cypher_astnode_t * const *items,
        unsigned int nitems, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the number of items in a `CYPHER_AST_REMOVE` node.
 *
 * If the node is not an instance of `CYPHER_AST_REMOVE` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return The number of items.
 */
__cypherlang_pure
unsigned int cypher_ast_remove_nitems(const cypher_astnode_t *node);

/**
 * Get an item from a `CYPHER_AST_REMOVE` node.
 *
 * If the node is not an instance of `CYPHER_AST_REMOVE` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the item.
 * @return A `CYPHER_AST_EXPRESSION` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_remove_get_item(
        const cypher_astnode_t *node, unsigned int index);


/**
 * Construct a `CYPHER_AST_REMOVE_LABELS` node.
 *
 * The node will also be an instance of `CYPHER_AST_REMOVE_ITEM`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [labels] Labels to remove, all of type `CYPHER_AST_LABEL`.
 * @param [nlabels] The number of labels.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_remove_labels(const cypher_astnode_t *identifier,
        cypher_astnode_t * const *labels, unsigned int nlabels,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the identifier of a `CYPHER_AST_REMOVE_LABELS` node.
 *
 * If the node is not an instance of `CYPHER_AST_REMOVE_LABELS` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_remove_labels_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the number of labels in a `CYPHER_AST_REMOVE_LABELS` node.
 *
 * If the node is not an instance of `CYPHER_AST_REMOVE_LABELS` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return The number of labels.
 */
__cypherlang_pure
unsigned int cypher_ast_remove_labels_nlabels(const cypher_astnode_t *node);

/**
 * Get a label from a `CYPHER_AST_REMOVE_LABELS` node.
 *
 * If the node is not an instance of `CYPHER_AST_REMOVE_LABELS` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the label.
 * @return A `CYPHER_AST_LABEL` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_remove_labels_get_label(
        const cypher_astnode_t *node, unsigned int index);


/**
 * Construct a `CYPHER_AST_REMOVE_PROPERTY` node.
 *
 * The node will also be an instance of `CYPHER_AST_REMOVE_ITEM`.
 *
 * @param [property] A `CYPHER_AST_PROPERTY_OPERATOR` node.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_remove_property(const cypher_astnode_t *property,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the property operator of a `CYPHER_AST_REMOVE_PROPERTY` node.
 *
 * If the node is not an instance of `CYPHER_AST_REMOVE_PROPERTY` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_PROPERTY_OPERATOR` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_remove_property_get_property(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_FOREACH` node.
 *
 * The node will also be an instance of `CYPHER_AST_CLAUSE`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [clauses] Clauses for foreach, all of type `CYPHER_AST_CLAUSE`.
 * @param [nclauses] The number of clauses.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_foreach(const cypher_astnode_t *identifier,
        const cypher_astnode_t *expression, cypher_astnode_t * const *clauses,
        unsigned int nclauses, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the identifier of a `CYPHER_AST_FOREACH` node.
 *
 * If the node is not an instance of `CYPHER_AST_FOREACH` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_foreach_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the expression of a `CYPHER_AST_FOREACH` node.
 *
 * If the node is not an instance of `CYPHER_AST_FOREACH` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_foreach_get_expression(
        const cypher_astnode_t *node);

/**
 * Get the number of clauses from a `CYPHER_AST_FOREACH` node.
 *
 * If the node is not an instance of `CYPHER_AST_FOREACH` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return The number of clauses.
 */
__cypherlang_pure
unsigned int cypher_ast_foreach_nclauses(const cypher_astnode_t *node);

/**
 * Get a clause from a `CYPHER_AST_FOREACH` node.
 *
 * If the node is not an instance of `CYPHER_AST_FOREACH` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the alternative.
 * @return A `CYPHER_AST_CLAUSE` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_foreach_get_clause(
        const cypher_astnode_t *node, unsigned int index);


/**
 * Construct a `CYPHER_AST_WITH` node.
 *
 * The node will also be an instance of `CYPHER_AST_CLAUSE`.
 *
 * @param [distinct] `true` if the `DISTINCT` keyword is specified.
 * @param [include_existing] `true` if including existing identifiers
 *         (i.e. `WITH *`).
 * @param [projections] Projections for with, all of type
 *         `CYPHER_AST_PROJECTION`.
 * @param [nprojections] The number of projections.
 * @param [order_by] A `CYPHER_AST_ORDER_BY` node, or null.
 * @param [skip] A `CYPHER_AST_INTEGER` node, or null.
 * @param [limit] A `CYPHER_AST_INTEGER` node, or null.
 * @param [predicate] A `CYPHER_AST_EXPRESSION` node, or null.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_with(bool distinct, bool include_existing,
        cypher_astnode_t * const *projections, unsigned int nprojections,
        const cypher_astnode_t *order_by, const cypher_astnode_t *skip,
        const cypher_astnode_t *limit, const cypher_astnode_t *predicate,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Check if `DISTINCT` is present for a `CYPHER_AST_WITH` node.
 *
 * If the node is not an instance of `CYPHER_AST_WITH` then the result will be
 * undefined.
 *
 * @param [node] The AST node.
 * @return `true` if `DISTINCT` is set, `false` otherwise.
 */
__cypherlang_pure
bool cypher_ast_with_is_distinct(const cypher_astnode_t *node);

/**
 * Check if including existing identifiers (`*`) in a `CYPHER_AST_WITH` node.
 *
 * If the node is not an instance of `CYPHER_AST_WITH` then the result will be
 * undefined.
 *
 * @param [node] The AST node.
 * @return `true` if including existing identifiers, `false` otherwise.
 */
__cypherlang_pure
bool cypher_ast_with_has_include_existing(const cypher_astnode_t *node);

/**
 * Get the number of projections in a `CYPHER_AST_WITH` node.
 *
 * If the node is not an instance of `CYPHER_AST_WITH` then the result will be
 * undefined.
 *
 * @param [node] The AST node.
 * @return The number of projections.
 */
__cypherlang_pure
unsigned int cypher_ast_with_nprojections(const cypher_astnode_t *node);

/**
 * Get a projection from a `CYPHER_AST_WITH` node.
 *
 * If the node is not an instance of `CYPHER_AST_WITH` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the identifier.
 * @return A `CYPHER_AST_PROJECTION` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_with_get_projection(
        const cypher_astnode_t *node, unsigned int index);

/**
 * Get the "order by" node of a `CYPHER_AST_WITH` node.
 *
 * If the node is not an instance of `CYPHER_AST_WITH` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_ORDER_BY` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_with_get_order_by(
        const cypher_astnode_t *node);

/**
 * Get the skip value of a `CYPHER_AST_WITH` node.
 *
 * If the node is not an instance of `CYPHER_AST_WITH` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_INTEGER` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_with_get_skip(
        const cypher_astnode_t *node);

/**
 * Get the limit value of a `CYPHER_AST_WITH` node.
 *
 * If the node is not an instance of `CYPHER_AST_WITH` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_INTEGER` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_with_get_limit(
        const cypher_astnode_t *node);

/**
 * Get the predicate of a `CYPHER_AST_WITH` node.
 *
 * If the node is not an instance of `CYPHER_AST_WITH` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_with_get_predicate(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_UNWIND` node.
 *
 * The node will also be an instance of `CYPHER_AST_CLAUSE`.
 *
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [alias] A `CYPHER_AST_IDENTIFIER` node.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_unwind(const cypher_astnode_t *expression,
        const cypher_astnode_t *alias, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the expression of a `CYPHER_AST_UNWIND` node.
 *
 * If the node is not an instance of `CYPHER_AST_UNWIND` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_unwind_get_expression(
        const cypher_astnode_t *node);

/**
 * Get the alias of a `CYPHER_AST_UNWIND` node.
 *
 * If the node is not an instance of `CYPHER_AST_UNWIND` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_unwind_get_alias(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_CALL` node.
 *
 * The node will also be an instance of `CYPHER_AST_CLAUSE`.
 *
 * @param [proc_name] A `CYPHER_AST_PROC_NAME` node.
 * @param [args] An array of `CYPHER_AST_EXPRESSION` nodes.
 * @param [nargs] The number of args.
 * @param [projections] Projections for with, all of type
 *         `CYPHER_AST_PROJECTION`.
 * @param [nprojections] The number of projections.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_call(const cypher_astnode_t *proc_name,
        cypher_astnode_t * const *args, unsigned int nargs,
        cypher_astnode_t * const *projections, unsigned int nprojections,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the proc name of a `CYPHER_AST_CALL` node.
 *
 * If the node is not an instance of `CYPHER_AST_CALL` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_PROC_NAME` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_call_get_proc_name(
        const cypher_astnode_t *node);

/**
 * Get the number of arguments of a `CYPHER_AST_CALL` node.
 *
 * If the node is not an instance of `CYPHER_AST_CALL` then the result will be
 * undefined.
 *
 * @param [node] The AST node.
 * @return The number of arguments.
 */
__cypherlang_pure
unsigned int cypher_ast_call_narguments(const cypher_astnode_t *node);

/**
 * Get an argument of a `CYPHER_AST_CALL` node.
 *
 * If the node is not an instance of `CYPHER_AST_CALL` then the result will be
 * undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the argument.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_call_get_argument(
        const cypher_astnode_t *node, unsigned int index);

/**
 * Get the number of projections of a `CYPHER_AST_CALL` node.
 *
 * If the node is not an instance of `CYPHER_AST_CALL` then the result will be
 * undefined.
 *
 * @param [node] The AST node.
 * @return The number of projections.
 */
__cypherlang_pure
unsigned int cypher_ast_call_nprojections(const cypher_astnode_t *node);

/**
 * Get a projection of a `CYPHER_AST_CALL` node.
 *
 * If the node is not an instance of `CYPHER_AST_CALL` then the result will be
 * undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the projection.
 * @return A `CYPHER_AST_PROJECTION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_call_get_projection(
        const cypher_astnode_t *node, unsigned int index);


/**
 * Construct a `CYPHER_AST_RETURN` node.
 *
 * The node will also be an instance of `CYPHER_AST_CLAUSE`.
 *
 * @param [distinct] `true` if the `DISTINCT` keyword is specified.
 * @param [include_existing] `true` if including existing identifiers
 *         (i.e. `RETURN *`).
 * @param [projections] Projections to return, all of type
 *         `CYPHER_AST_PROJECTION`.
 * @param [nprojections] The number of projections.
 * @param [order_by] A `CYPHER_AST_ORDER_BY` node, or null.
 * @param [skip] A `CYPHER_AST_INTEGER` node, or null.
 * @param [limit] A `CYPHER_AST_INTEGER` node, or null.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_return(bool distinct, bool include_existing,
        cypher_astnode_t * const *projections, unsigned int nprojections,
        const cypher_astnode_t *order_by, const cypher_astnode_t *skip,
        const cypher_astnode_t *limit, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Check if `DISTINCT` is set in a `CYPHER_AST_RETURN` node.
 *
 * If the node is not an instance of `CYPHER_AST_RETURN` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return `true` if `DISTINCT` is set, `false` otherwise.
 */
__cypherlang_pure
bool cypher_ast_return_is_distinct(const cypher_astnode_t *node);

/**
 * Check if including existing identifiers (`*`) in a `CYPHER_AST_RETURN` node.
 *
 * If the node is not an instance of `CYPHER_AST_RETURN` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return `true` if including existing identifiers, `false` otherwise.
 */
__cypherlang_pure
bool cypher_ast_return_has_include_existing(const cypher_astnode_t *node);

/**
 * Get the number of projections in a `CYPHER_AST_RETURN` node.
 *
 * If the node is not an instance of `CYPHER_AST_RETURN` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return The number of projections.
 */
__cypherlang_pure
unsigned int cypher_ast_return_nprojections(const cypher_astnode_t *node);

/**
 * Get a projection from a `CYPHER_AST_RETURN` node.
 *
 * If the node is not an instance of `CYPHER_AST_RETURN` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the identifier.
 * @return A `CYPHER_AST_PROJECTION` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_return_get_projection(
        const cypher_astnode_t *node, unsigned int index);

/**
 * Get the "order by" node of a `CYPHER_AST_RETURN` node.
 *
 * If the node is not an instance of `CYPHER_AST_RETURN` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_ORDER_BY` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_return_get_order_by(
        const cypher_astnode_t *node);

/**
 * Get the skip value of a `CYPHER_AST_RETURN` node.
 *
 * If the node is not an instance of `CYPHER_AST_RETURN` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_INTEGER` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_return_get_skip(
        const cypher_astnode_t *node);

/**
 * Get the limit value of a `CYPHER_AST_RETURN` node.
 *
 * If the node is not an instance of `CYPHER_AST_RETURN` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_INTEGER` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_return_get_limit(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_PROJECTION` node.
 *
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [alias] A `CYPHER_AST_IDENTIFIER` node, or null.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_projection(const cypher_astnode_t *expression,
        const cypher_astnode_t *alias, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the expression from a `CYPHER_AST_PROJECTION` node.
 *
 * If the node is not an instance of `CYPHER_AST_PROJECTION` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_projection_get_expression(
        const cypher_astnode_t *node);

/**
 * Get the alias from a `CYPHER_AST_PROJECTION` node.
 *
 * If the node is not an instance of `CYPHER_AST_PROJECTION` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_projection_get_alias(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_ORDER_BY` node.
 *
 * @param [items] Items for `ORDER BY`, all of type `CYPHER_AST_SORT_ITEM`.
 * @param [nitems] The number of items.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_order_by(cypher_astnode_t * const *items,
        unsigned int nitems, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the number of items in a `CYPHER_AST_ORDER_BY` node.
 *
 * If the node is not an instance of `CYPHER_AST_ORDER_BY` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return The number of items.
 */
__cypherlang_pure
unsigned int cypher_ast_order_by_nitems(const cypher_astnode_t *node);

/**
 * Get an item from a `CYPHER_AST_ORDER_BY` node.
 *
 * If the node is not an instance of `CYPHER_AST_ORDER_BY` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the action.
 * @return A `CYPHER_AST_SET_ITEM` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_order_by_get_item(
        const cypher_astnode_t *node, unsigned int index);


/**
 * Construct a `CYPHER_AST_SORT_ITEM` node.
 *
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [ascending] `true` if the sort item is ascending.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_sort_item(const cypher_astnode_t *expression,
        bool ascending, cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the expression from a `CYPHER_AST_SORT_ITEM` node.
 *
 * If the node is not an instance of `CYPHER_AST_SORT_ITEM` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_sort_item_get_expression(
        const cypher_astnode_t *node);

/**
 * Check if a `CYPHER_AST_SORT_ITEM` node is using ascending order.
 *
 * If the node is not an instance of `CYPHER_AST_SORT_ITEM` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return `true` if the sort item is ascending, `false` otherwise.
 */
__cypherlang_pure
bool cypher_ast_sort_item_is_ascending(const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_UNION` node.
 *
 * The node will also be an instance of `CYPHER_AST_CLAUSE`.
 *
 * @param [all] `true` if `ALL` is specified for the union.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_union(bool all, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Check if a `CYPHER_AST_UNION` node has `ALL` specified.
 *
 * If the node is not an instance of `CYPHER_AST_UNION` then the result will be
 * undefined.
 *
 * @param [node] The AST node.
 * @return `true` if `ALL` is specified, `false` otherwise.
 */
__cypherlang_pure
bool cypher_ast_union_has_all(const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_UNARY_OPERATOR` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [op] The operator.
 * @param [arg] The argument to the unary operator, of type
 *         `CYPHER_AST_EXPRESSION`.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_unary_operator(const cypher_operator_t *op,
        const cypher_astnode_t *arg, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the operator of a `CYPHER_AST_UNARY_OPERATOR` node.
 *
 * If the node is not an instance of `CYPHER_AST_UNARY_OPERATOR` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return The operator.
 */
__cypherlang_pure
const cypher_operator_t *cypher_ast_unary_operator_get_operator(
        const cypher_astnode_t *node);

/**
 * Get the argument of a `CYPHER_AST_UNARY_OPERATOR` node.
 *
 * If the node is not an instance of `CYPHER_AST_UNARY_OPERATOR` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return The argument, of type `CYPHER_AST_EXPRESSION`.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_unary_operator_get_argument(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_BINARY_OPERATOR` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [op] The operator.
 * @param [arg1] The first (left) argument to the binary operator, of type
 *         `CYPHER_AST_EXPRESSION`.
 * @param [arg2] The second (right) argument to the binary operator, of type
 *         `CYPHER_AST_EXPRESSION`.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_binary_operator(const cypher_operator_t *op,
        const cypher_astnode_t *arg1, const cypher_astnode_t *arg2,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the operator of a `CYPHER_AST_BINARY_OPERATOR` node.
 *
 * If the node is not an instance of `CYPHER_AST_BINARY_OPERATOR` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return The operator.
 */
__cypherlang_pure
const cypher_operator_t *cypher_ast_binary_operator_get_operator(
        const cypher_astnode_t *node);

/**
 * Get the first (left) argument of a `CYPHER_AST_BINARY_OPERATOR` node.
 *
 * If the node is not an instance of `CYPHER_AST_BINARY_OPERATOR` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return The argument, of type `CYPHER_AST_EXPRESSION`.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_binary_operator_get_argument1(
        const cypher_astnode_t *node);

/**
 * Get the second (right) argument of a `CYPHER_AST_BINARY_OPERATOR` node.
 *
 * If the node is not an instance of `CYPHER_AST_BINARY_OPERATOR` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return The argument, of type `CYPHER_AST_EXPRESSION`.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_binary_operator_get_argument2(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_COMPARISON` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [length] The length of the comparison chain.
 * @param [ops] The operators in the comparison chain, which must be
 *         length pointers.
 * @param [args] The arguments in the comparison chain, which must be
 *         length+1 pointers all of type `CYPHER_AST_EXPRESSION`.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_comparison(unsigned int length,
        const cypher_operator_t * const *ops, cypher_astnode_t * const *args,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the comparsion chain length of a `CYPHER_AST_COMPARISON` node.
 *
 * If the node is not an instance of `CYPHER_AST_COMPARISON` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return The length of the chain.
 */
unsigned int cypher_ast_comparison_get_length(const cypher_astnode_t *node);

/**
 * Get an operator in the chain of a `CYPHER_AST_COMPARISON` node.
 *
 * If the node is not an instance of `CYPHER_AST_COMPARISON` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @param [pos] The position in the comparison chain.
 * @return The operator at the specified position in the chain, or null if
 *         the position is greater than the chain length.
 */
__cypherlang_pure
const cypher_operator_t *cypher_ast_comparison_get_operator(
        const cypher_astnode_t *node, unsigned int pos);

/**
 * Get the argument in the chain of a `CYPHER_AST_COMPARISON` node.
 *
 * If the node is not an instance of `CYPHER_AST_COMPARISON` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @param [pos] The position in the comparison chain.
 * @return A `CYPHER_AST_EXPRESSION` node at the specified position in the
 *         chain, or null if the position is greater than the chain length.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_comparison_get_argument(
        const cypher_astnode_t *node, unsigned int pos);


/**
 * Construct a `CYPHER_AST_APPLY_OPERATOR` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [func_name] A `CYPHER_AST_FUNCTION_NAME` node.
 * @param [distinct] `true` if the `DISTINCT` keyword is specified.
 * @param [args] An array of `CYPHER_AST_EXPRESSION` nodes.
 * @param [nargs] The number of args.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_apply_operator(const cypher_astnode_t *func_name,
        bool distinct, cypher_astnode_t * const *args, unsigned int nargs,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the function name of a `CYPHER_AST_APPLY_OPERATOR` node.
 *
 * If the node is not an instance of `CYPHER_AST_APPLY_OPERATOR` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_FUNCTION_NAME` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_apply_operator_get_func_name(
        const cypher_astnode_t *node);

/**
 * Check if a `CYPHER_AST_APPLY_OPERATOR` node is `DISTINCT`.
 *
 * If the node is not an instance of `CYPHER_AST_APPLY_OPERATOR` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return `true` if `DISTINCT` is specified, and `false` otherwise.
 */
__cypherlang_pure
bool cypher_ast_apply_operator_get_distinct(const cypher_astnode_t *node);

/**
 * Get the number of arguments of a `CYPHER_AST_APPLY_OPERATOR` node.
 *
 * If the node is not an instance of `CYPHER_AST_APPLY_OPERATOR` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return The number of arguments.
 */
__cypherlang_pure
unsigned int cypher_ast_apply_operator_narguments(const cypher_astnode_t *node);

/**
 * Get an argument of a `CYPHER_AST_APPLY_OPERATOR` node.
 *
 * If the node is not an instance of `CYPHER_AST_APPLY_OPERATOR` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the argument.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_apply_operator_get_argument(
        const cypher_astnode_t *node, unsigned int index);


/**
 * Construct a `CYPHER_AST_APPLY_ALL_OPERATOR` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [func_name] A `CYPHER_AST_FUNCTION_NAME` node.
 * @param [distinct] `true` if the function removes duplicate results.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_apply_all_operator(
        const cypher_astnode_t *func_name,
        bool distinct, cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the function name of a `CYPHER_AST_APPLY_ALL_OPERATOR` node.
 *
 * If the node is not an instance of `CYPHER_AST_APPLY_ALL_OPERATOR` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_FUNCTION_NAME` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_apply_all_operator_get_func_name(
        const cypher_astnode_t *node);

/**
 * Check if a `CYPHER_AST_APPLY_ALL_OPERATOR` node is `DISTINCT`.
 *
 * If the node is not an instance of `CYPHER_AST_APPLY_ALL_OPERATOR` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return `true` if `DISTINCT` is specified, and `false` otherwise.
 */
__cypherlang_pure
bool cypher_ast_apply_all_operator_get_distinct(const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_PROPERTY_OPERATOR` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [prop_name] A `CYPHER_AST_PROP_NAME` node.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_property_operator(
        const cypher_astnode_t *expression, const cypher_astnode_t *prop_name,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the expression of a `CYPHER_AST_PROPERTY_OPERATOR` node.
 *
 * If the node is not an instance of `CYPHER_AST_PROPERTY_OPERATOR` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_property_operator_get_expression(
        const cypher_astnode_t *node);

/**
 * Get the property name of a `CYPHER_AST_PROPERTY_OPERATOR` node.
 *
 * If the node is not an instance of `CYPHER_AST_PROPERTY_OPERATOR` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_PROP_NAME` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_property_operator_get_prop_name(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_SUBSCRIPT_OPERATOR` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [subscript] A `CYPHER_AST_EXPRESSION` node.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_subscript_operator(
        const cypher_astnode_t *expression, const cypher_astnode_t *subscript,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the expression of a `CYPHER_AST_SUBSCRIPT_OPERATOR` node.
 *
 * If the node is not an instance of `CYPHER_AST_SUBSCRIPT_OPERATOR` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_subscript_operator_get_expression(
        const cypher_astnode_t *node);

/**
 * Get the subscript of a `CYPHER_AST_SUBSCRIPT_OPERATOR` node.
 *
 * If the node is not an instance of `CYPHER_AST_SUBSCRIPT_OPERATOR` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_subscript_operator_get_subscript(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_SLICE_OPERATOR` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [start] A `CYPHER_AST_EXPRESSION` node, or null.
 * @param [end] A `CYPHER_AST_EXPRESSION` node, or null.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_slice_operator(const cypher_astnode_t *expression,
        const cypher_astnode_t *start, const cypher_astnode_t *end,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the expression of a `CYPHER_AST_SLICE_OPERATOR` node.
 *
 * If the node is not an instance of `CYPHER_AST_SLICE_OPERATOR` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_slice_operator_get_expression(
        const cypher_astnode_t *node);

/**
 * Get the start of a `CYPHER_AST_SLICE_OPERATOR` node.
 *
 * If the node is not an instance of `CYPHER_AST_SLICE_OPERATOR` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_slice_operator_get_start(
        const cypher_astnode_t *node);

/**
 * Get the end of a `CYPHER_AST_SLICE_OPERATOR` node.
 *
 * If the node is not an instance of `CYPHER_AST_SLICE_OPERATOR` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_slice_operator_get_end(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_LABELS_OPERATOR` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [labels] Labels for the operator, all of type `CYPHER_AST_LABEL`.
 * @param [nlabels] The number of labels.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_labels_operator(const cypher_astnode_t *expression,
        cypher_astnode_t * const *labels, unsigned int nlabels,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the expression of a `CYPHER_AST_PROPERTY_OPERATOR` node.
 *
 * If the node is not an instance of `CYPHER_AST_PROPERTY_OPERATOR` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_labels_operator_get_expression(
        const cypher_astnode_t *node);

/**
 * Get the number of labels from a `CYPHER_AST_LABELS_OPERATOR` node.
 *
 * If the node is not an instance of `CYPHER_AST_LABELS_OPERATOR` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return The number of labels.
 */
__cypherlang_pure
unsigned int cypher_ast_labels_operator_nlabels(const cypher_astnode_t *node);

/**
 * Get a label from a `CYPHER_AST_LABELS_OPERATOR` node.
 *
 * If the node is not an instance of `CYPHER_AST_LABELS_OPERATOR` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the label.
 * @return A `CYPHER_AST_LABEL` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_labels_operator_get_label(
        const cypher_astnode_t *node, unsigned int index);


/**
 * Construct a `CYPHER_AST_LIST_COMPREHENSION` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [predicate] A `CYPHER_AST_EXPRESSION` node, or null.
 * @param [eval] A `CYPHER_AST_EXPRESSION` node, or null.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_list_comprehension(
        const cypher_astnode_t *identifier, const cypher_astnode_t *expression,
        const cypher_astnode_t *predicate, const cypher_astnode_t *eval,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the identifier from a `CYPHER_AST_LIST_COMPREHENSION` node.
 *
 * If the node is not an instance of `CYPHER_AST_LIST_COMPREHENSION` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_list_comprehension_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the expression from a `CYPHER_AST_LIST_COMPREHENSION` node.
 *
 * If the node is not an instance of `CYPHER_AST_LIST_COMPREHENSION` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_list_comprehension_get_expression(
        const cypher_astnode_t *node);

/**
 * Get the predicate from a `CYPHER_AST_LIST_COMPREHENSION` node.
 *
 * If the node is not an instance of `CYPHER_AST_LIST_COMPREHENSION` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_list_comprehension_get_predicate(
        const cypher_astnode_t *node);

/**
 * Get the evaluation from a `CYPHER_AST_LIST_COMPREHENSION` node.
 *
 * If the node is not an instance of `CYPHER_AST_LIST_COMPREHENSION` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_list_comprehension_get_eval(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_FILTER` node.
 *
 * The node will also be an instance of `CYPHER_AST_LIST_COMPREHENSION`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [predicate] A `CYPHER_AST_EXPRESSION` node, or null.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_filter(const cypher_astnode_t *identifier,
        const cypher_astnode_t *expression, const cypher_astnode_t *predicate,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);


/**
 * Construct a `CYPHER_AST_EXTRACT` node.
 *
 * The node will also be an instance of `CYPHER_AST_LIST_COMPREHENSION`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [eval] A `CYPHER_AST_EXPRESSION` node, or null.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_extract(const cypher_astnode_t *identifier,
        const cypher_astnode_t *expression, const cypher_astnode_t *eval,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);


/**
 * Construct a `CYPHER_AST_ALL` node.
 *
 * The node will also be an instance of `CYPHER_AST_LIST_COMPREHENSION`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [predicate] A `CYPHER_AST_EXPRESSION` node, or null.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_all(const cypher_astnode_t *identifier,
        const cypher_astnode_t *expression, const cypher_astnode_t *predicate,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);


/**
 * Construct a `CYPHER_AST_ANY` node.
 *
 * The node will also be an instance of `CYPHER_AST_LIST_COMPREHENSION`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [predicate] A `CYPHER_AST_EXPRESSION` node, or null.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_any(const cypher_astnode_t *identifier,
        const cypher_astnode_t *expression, const cypher_astnode_t *predicate,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);


/**
 * Construct a `CYPHER_AST_SINGLE` node.
 *
 * The node will also be an instance of `CYPHER_AST_LIST_COMPREHENSION`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [predicate] A `CYPHER_AST_EXPRESSION` node, or null.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_single(const cypher_astnode_t *identifier,
        const cypher_astnode_t *expression, const cypher_astnode_t *predicate,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);


/**
 * Construct a `CYPHER_AST_NONE` node.
 *
 * The node will also be an instance of `CYPHER_AST_LIST_COMPREHENSION`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [predicate] A `CYPHER_AST_EXPRESSION` node, or null.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_none(const cypher_astnode_t *identifier,
        const cypher_astnode_t *expression, const cypher_astnode_t *predicate,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);


/**
 * Construct a `CYPHER_AST_REDUCE` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [accumulator] A `CYPHER_AST_IDENTIFIER` node.
 * @param [init] A `CYPHER_AST_EXPRESSION` node.
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [eval] A `CYPHER_AST_EXPRESSION` node, or null.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_reduce(const cypher_astnode_t *accumulator,
        const cypher_astnode_t *init, const cypher_astnode_t *identifier,
        const cypher_astnode_t *expression, const cypher_astnode_t *eval,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the accumulator from a `CYPHER_AST_REDUCE` node.
 *
 * If the node is not an instance of `CYPHER_AST_REDUCE` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_reduce_get_accumulator(
        const cypher_astnode_t *node);

/**
 * Get the accumulator init expression from a `CYPHER_AST_REDUCE` node.
 *
 * If the node is not an instance of `CYPHER_AST_REDUCE` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_reduce_get_init(
        const cypher_astnode_t *node);

/**
 * Get the identifier from a `CYPHER_AST_REDUCE` node.
 *
 * If the node is not an instance of `CYPHER_AST_REDUCE` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_reduce_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the expression from a `CYPHER_AST_REDUCE` node.
 *
 * If the node is not an instance of `CYPHER_AST_REDUCE` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_reduce_get_expression(
        const cypher_astnode_t *node);

/**
 * Get the evaluation from a `CYPHER_AST_REDUCE` node.
 *
 * If the node is not an instance of `CYPHER_AST_REDUCE` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_reduce_get_eval(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_CASE` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [expression] A `CYPHER_AST_EXPRESSION` node.
 * @param [alternatives] An alternating list of predicate expressions and
 *         values, all of type `CYPHER_AST_EXPRESSION`.
 * @param [nalternatives] The number of predicate and result pairs.
 * @param [deflt] A `CYPHER_AST_EXPRESSION` node, or `NULL`.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_case(const cypher_astnode_t *expression,
        cypher_astnode_t * const *alternatives, unsigned int nalternatives,
        const cypher_astnode_t *deflt, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the expression from a `CYPHER_AST_CASE` node.
 *
 * If the node is not an instance of `CYPHER_AST_CASE` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_case_get_expression(
        const cypher_astnode_t *node);

/**
 * Get the number of alternatives from a `CYPHER_AST_CASE` node.
 *
 * If the node is not an instance of `CYPHER_AST_CASE` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return The number of alternatives.
 */
__cypherlang_pure
unsigned int cypher_ast_case_nalternatives(const cypher_astnode_t *node);

/**
 * Get a predicate from a `CYPHER_AST_CASE` node.
 *
 * If the node is not an instance of `CYPHER_AST_CASE` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the alternative.
 * @return A `CYPHER_AST_EXPRESSION` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_case_get_predicate(
        const cypher_astnode_t *node, unsigned int index);

/**
 * Get a value from a `CYPHER_AST_CASE` node.
 *
 * If the node is not an instance of `CYPHER_AST_CASE` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the alternative.
 * @return A `CYPHER_AST_EXPRESSION` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_case_get_value(
        const cypher_astnode_t *node, unsigned int index);

/**
 * Get the default from a `CYPHER_AST_CASE` node.
 *
 * If the node is not an instance of `CYPHER_AST_CASE` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_EXPRESSION` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_case_get_default(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_COLLECTION` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [elements] The elements for the collection.
 * @param [nelements] The number of elements.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_collection(
        cypher_astnode_t * const *elements, unsigned int nelements,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the collection length from a `CYPHER_AST_COLLECTION` node.
 *
 * If the node is not an instance of `CYPHER_AST_COLLECTION` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return The length of the collection.
 */
__cypherlang_pure
unsigned int cypher_ast_collection_length(const cypher_astnode_t *node);

/**
 * Get an element from a `CYPHER_AST_COLLECTION` node.
 *
 * If the node is not an instance of `CYPHER_AST_COLLECTION` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the element.
 * @return A pointer to the element of the collection, or `NULL` if there is no
 *         element at the specified index.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_collection_get(const cypher_astnode_t *node,
        unsigned int index);


/**
 * Construct a `CYPHER_AST_MAP` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [keys] The keys for the map, all of type `CYPHER_AST_PROP_NAME`.
 * @param [values] The values for the map, all of type `CYPEHR_AST_EXPRESSION`.
 * @param [nentries] The number of keys and values.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
cypher_astnode_t *cypher_ast_map(cypher_astnode_t * const *keys,
        cypher_astnode_t * const *values, unsigned int nentries,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Construct a `CYPHER_AST_MAP` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [pairs] An alternating list of keys for the map, of type
 *         `CYPHER_AST_PROP_NAME`, and values for the map, of type
 *         `CYPHER_AST_EXPRESSION`.
 * @param [nentries] The number of pairs (i.e. `pairs` must contain
 *         `nentries*2` pointers).
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_pair_map(cypher_astnode_t * const *pairs,
        unsigned int nentries, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the number of entries in a `CYPHER_AST_MAP` node.
 *
 * If the node is not an instance of `CYPHER_AST_MAP` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return The number of entries.
 */
__cypherlang_pure
unsigned int cypher_ast_map_nentries(const cypher_astnode_t *node);

/**
 * Get a key from a `CYPHER_AST_MAP` node.
 *
 * If the node is not an instance of `CYPHER_AST_MAP` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the entry.
 * @return A `CYPHER_AST_PROP_NAME` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_map_get_key(const cypher_astnode_t *node,
        unsigned int index);

/**
 * Get a value from a `CYPHER_AST_MAP` node.
 *
 * If the node is not an instance of `CYPHER_AST_MAP` then the
 * result will be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the entry.
 * @return A `CYPHER_AST_EXPRESSION` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_map_get_value(
        const cypher_astnode_t *node, unsigned int index);


/**
 * Construct a `CYPHER_AST_IDENTIFIER` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [s] A pointer to a character string containing the identifier name.
 * @param [n] The length of the character string.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_identifier(const char *s, size_t n,
        struct cypher_input_range range);

/**
 * Get a null terminated name string for a `CYPHER_AST_IDENTIFIER` node.
 *
 * If the node is not an instance of `CYPHER_AST_IDENTIFIER` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A pointer to the null terminated string.
 */
__cypherlang_pure
const char *cypher_ast_identifier_get_name(const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_PARAMETER` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [s] A pointer to a character string containing the identifier name.
 * @param [n] The length of the character string.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_parameter(const char *s, size_t n,
        struct cypher_input_range range);

/**
 * Get a null terminated name string for a `CYPHER_AST_PARAMETER` node.
 *
 * If the node is not an instance of `CYPHER_AST_PARAMETER` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A pointer to the null terminated string.
 */
__cypherlang_pure
const char *cypher_ast_parameter_get_name(const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_STRING` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [s] A pointer to a character string.
 * @param [n] The length of the character string.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_string(const char *s, size_t n,
        struct cypher_input_range range);

/**
 * Get the null terminated string value from a `CYPHER_AST_STRING` node.
 *
 * If the node is not an instance of `CYPHER_AST_STRING` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A pointer to the null terminated string.
 */
__cypherlang_pure
const char *cypher_ast_string_get_value(const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_INTEGER` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [s] A pointer to a character string.
 * @param [n] The length of the character string.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_integer(const char *s, size_t n,
        struct cypher_input_range range);

/**
 * Get the null terminated string value from a `CYPHER_AST_INTEGER` node.
 *
 * If the node is not an instance of `CYPHER_AST_INTEGER` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A pointer to the null terminated string.
 */
__cypherlang_pure
const char *cypher_ast_integer_get_valuestr(const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_FLOAT` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [s] A pointer to a character string.
 * @param [n] The length of the character string.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_float(const char *s, size_t n,
        struct cypher_input_range range);

/**
 * Get the null terminated string value from a `CYPHER_AST_FLOAT` node.
 *
 * If the node is not an instance of `CYPHER_AST_FLOAT` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A pointer to the null terminated string.
 */
__cypherlang_pure
const char *cypher_ast_float_get_valuestr(const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_TRUE` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_true(struct cypher_input_range range);


/**
 * Construct a `CYPHER_AST_FALSE` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_false(struct cypher_input_range range);


/**
 * Construct a `CYPHER_AST_NULL` node.
 *
 * The node will also be an instance of `CYPHER_AST_EXPRESSION`.
 *
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_null(struct cypher_input_range range);


/**
 * Construct a `CYPHER_AST_LABEL` node.
 *
 * @param [s] A pointer to a character string containing the label name.
 * @param [n] The length of the character string.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_label(const char *s, size_t n,
        struct cypher_input_range range);

/**
 * Get a null terminated name string for a `CYPHER_AST_LABEL` node.
 *
 * If the node is not an instance of `CYPHER_AST_LABEL` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A pointer to the null terminated string.
 */
__cypherlang_pure
const char *cypher_ast_label_get_name(const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_RELTYPE` node.
 *
 * @param [s] A pointer to a character string containing the label name.
 * @param [n] The length of the character string.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_reltype(const char *s, size_t n,
        struct cypher_input_range range);

/**
 * Get a null terminated name string for a `CYPHER_AST_RELTYPE` node.
 *
 * If the node is not an instance of `CYPHER_AST_RELTYPE` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A pointer to the null terminated string.
 */
__cypherlang_pure
const char *cypher_ast_reltype_get_name(const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_PROP_NAME` node.
 *
 * @param [s] A pointer to a character string containing the property name.
 * @param [n] The length of the character string.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_prop_name(const char *s, size_t n,
        struct cypher_input_range range);

/**
 * Get a null terminated name string for a `CYPHER_AST_PROP_NAME` node.
 *
 * If the node is not an instance of `CYPHER_AST_PROP_NAME` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A pointer to the null terminated string.
 */
__cypherlang_pure
const char *cypher_ast_prop_name_get_value(const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_FUNCTION_NAME` node.
 *
 * @param [s] A pointer to a character string containing the function name.
 * @param [n] The length of the character string.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_function_name(const char *s, size_t n,
        struct cypher_input_range range);

/**
 * Get a null terminated name string for a `CYPHER_AST_FUNCTION_NAME` node.
 *
 * If the node is not an instance of `CYPHER_AST_FUNCTION_NAME` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A pointer to the null terminated string.
 */
__cypherlang_pure
const char *cypher_ast_function_name_get_value(const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_INDEX_NAME` node.
 *
 * @param [s] A pointer to a character string containing the index name.
 * @param [n] The length of the character string.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_index_name(const char *s, size_t n,
        struct cypher_input_range range);

/**
 * Get a null terminated name string for a `CYPHER_AST_INDEX_NAME` node.
 *
 * If the node is not an instance of `CYPHER_AST_INDEX_NAME` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A pointer to the null terminated string.
 */
__cypherlang_pure
const char *cypher_ast_index_name_get_value(const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_PROC_NAME` node.
 *
 * @param [s] A pointer to a character string containing the proc name.
 * @param [n] The length of the character string.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_proc_name(const char *s, size_t n,
        struct cypher_input_range range);

/**
 * Get a null terminated name string for a `CYPHER_AST_PROC_NAME` node.
 *
 * If the node is not an instance of `CYPHER_AST_PROC_NAME` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A pointer to the null terminated string.
 */
__cypherlang_pure
const char *cypher_ast_proc_name_get_value(const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_PATTERN` node.
 *
 * @param [paths] The paths in the pattern, which must be all of type
 *         `CYPHER_AST_PATTERN_PATH`.
 * @param [npaths] The number of paths.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_pattern(cypher_astnode_t * const *paths,
        unsigned int npaths, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the number of paths in a `CYPHER_AST_PATTERN` node.
 *
 * If the node is not an instance of `CYPHER_AST_PATTERN` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return The number of paths.
 */
__cypherlang_pure
unsigned int cypher_ast_pattern_npaths(const cypher_astnode_t *node);

/**
 * Get a path from a `CYPHER_AST_PATTERN` node.
 *
 * If the node is not an instance of `CYPHER_AST_PATTERN` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the path.
 * @return A `CYPHER_AST_PATTERN_PATH` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_pattern_get_path(
        const cypher_astnode_t *node, unsigned int index);


/**
 * Construct a `CYPHER_AST_NAMED_PATH` node.
 *
 * The node will also be an instance of `CYPHER_AST_PATTERN_PATH`.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node.
 * @param [path] A `CYPHER_AST_PATTERN_PATH` node.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_named_path(const cypher_astnode_t *identifier,
        const cypher_astnode_t *path, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the identifier from a `CYPHER_AST_NAMED_PATH` node.
 *
 * If the node is not an instance of `CYPHER_AST_NAMED_PATH` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_named_path_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the unnamed pattern path from a `CYPHER_AST_NAMED_PATH` node.
 *
 * If the node is not an instance of `CYPHER_AST_NAMED_PATH` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_PATTERN_PATH` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_named_path_get_path(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_SHORTEST_PATH` node.
 *
 * The node will also be an instance of `CYPHER_AST_PATTERN_PATH` and
 * `CYPHER_AST_EXPRESSION`.
 *
 * @param [single] true if `shortestPath`, and false if `allShortestPaths`.
 * @param [path] A `CYPHER_AST_PATTERN_PATH` node.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_shortest_path(bool single,
        const cypher_astnode_t *path, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Check if a `CYPHER_AST_SHORTEST_PATH` node is for a single path.
 *
 * If the node is not an instance of `CYPHER_AST_SHORTEST_PATH` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return true if `shortestPath`, and false if `allShortestPaths`.
 */
__cypherlang_pure
bool cypher_ast_shortest_path_is_single(const cypher_astnode_t *node);

/**
 * Get the unnamed pattern path from a `CYPHER_AST_SHORTEST_PATH` node.
 *
 * If the node is not an instance of `CYPHER_AST_SHORTEST_PATH` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_PATTERN_PATH` node.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_shortest_path_get_path(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_PATTERN_PATH` node.
 *
 * @param [elements] The elements in the pattern, which must be of alternating
 *         types `CYPHER_AST_NODE_PATTERN` and `CYPHER_AST_REL_PATTERN`.
 * @param [nelements] The number of elements in the path, which must be odd.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_pattern_path(cypher_astnode_t * const *elements,
        unsigned int nelements, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the number of elements in a `CYPHER_AST_PATTERN_PATH` node.
 *
 * If the node is not an instance of `CYPHER_AST_PATTERN_PATH` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return The number of elements, which will be odd.
 */
__cypherlang_pure
unsigned int cypher_ast_pattern_path_nelements(const cypher_astnode_t *node);

/**
 * Get an element from a `CYPHER_AST_PATTERN_PATH` node.
 *
 * If the node is not an instance of `CYPHER_AST_PATTERN_PATH` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the element.
 * @return Either a `CYPHER_AST_NODE_PATTERN` node or a
 *         `CYPHER_AST_REL_PATTERN` node, depending on whether the index
 *         is even or odd respectively, or null if the index is larger than the
 *         number of elements..
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_pattern_path_get_element(
        const cypher_astnode_t *node, unsigned int index);


/**
 * Construct a `CYPHER_AST_NODE_PATTERN` node.
 *
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node, or null.
 * @param [labels] The labels in the pattern, which must all be of type
 *         `CYPHER_AST_LABEL`.
 * @param [nlabels] The number of labels in the pattern.
 * @param [properties] A `CYPHER_AST_MAP` node, a `CYPHER_AST_PARAMETER` node,
 *         or null.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_node_pattern(const cypher_astnode_t *identifier,
        cypher_astnode_t * const *labels, unsigned int nlabels,
        const cypher_astnode_t *properties, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the identifier of a `CYPHER_AST_NODE_PATTERN` node.
 *
 * If the node is not an instance of `CYPHER_AST_NODE_PATTERN` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_node_pattern_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the number of labels in a `CYPHER_AST_NODE_PATTERN` node.
 *
 * If the node is not an instance of `CYPHER_AST_NODE_PATTERN` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return The number of labels.
 */
__cypherlang_pure
unsigned int cypher_ast_node_pattern_nlabels(const cypher_astnode_t *node);

/**
 * Get a label from a `CYPHER_AST_NODE_PATTERN` node.
 *
 * If the node is not an instance of `CYPHER_AST_NODE_PATTERN` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the label.
 * @return A `CYPHER_AST_LABEL` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_node_pattern_get_label(
        const cypher_astnode_t *node, unsigned int index);

/**
 * Get the properties of a `CYPHER_AST_NODE_PATTERN` node.
 *
 * If the node is not an instance of `CYPHER_AST_NODE_PATTERN` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_MAP` node, a `CYPHER_AST_PARAMETER` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_node_pattern_get_properties(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_REL_PATTERN` node.
 *
 * @param [direction] The direction of the relationship.
 * @param [identifier] A `CYPHER_AST_IDENTIFIER` node, or null.
 * @param [reltypes] The relationship types in the pattern, all of type
 *         `CYPHER_AST_RELTYPE`.
 * @param [nreltypes] The number of relationship types in the pattern.
 * @param [properties] A `CYPHER_AST_MAP` node, a `CYPHER_AST_PARAMETER` node,
 *         or null.
 * @param [varlength] A `CYPHER_AST_RANGE` node, or null.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_rel_pattern(enum cypher_rel_direction direction,
        const cypher_astnode_t *identifier, cypher_astnode_t * const *reltypes,
        unsigned int nreltypes, const cypher_astnode_t *properties,
        const cypher_astnode_t *varlength, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);


/**
 * Get the direction of a `CYPHER_AST_REL_PATTERN` node.
 *
 * If the node is not an instance of `CYPHER_AST_REL_PATTERN` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return The direction of the relationship.
 */
__cypherlang_pure
enum cypher_rel_direction cypher_ast_rel_pattern_get_direction(
        const cypher_astnode_t *node);

/**
 * Get the identifier of a `CYPHER_AST_REL_PATTERN` node.
 *
 * If the node is not an instance of `CYPHER_AST_REL_PATTERN` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_IDENTIFIER` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_rel_pattern_get_identifier(
        const cypher_astnode_t *node);

/**
 * Get the number of reltypes in a `CYPHER_AST_REL_PATTERN` node.
 *
 * If the node is not an instance of `CYPHER_AST_REL_PATTERN` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return The number of reltypes.
 */
__cypherlang_pure
unsigned int cypher_ast_rel_pattern_nreltypes(const cypher_astnode_t *node);

/**
 * Get a reltypes from a `CYPHER_AST_REL_PATTERN` node.
 *
 * If the node is not an instance of `CYPHER_AST_REL_PATTERN` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the reltypes.
 * @return A `CYPHER_AST_LABEL` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_rel_pattern_get_reltype(
        const cypher_astnode_t *node, unsigned int index);

/**
 * Get the varlength range of a `CYPHER_AST_REL_PATTERN` node.
 *
 * If the node is not an instance of `CYPHER_AST_REL_PATTERN` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_RANGE` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_rel_pattern_get_varlength(
        const cypher_astnode_t *node);

/**
 * Get the properties of a `CYPHER_AST_REL_PATTERN` node.
 *
 * If the node is not an instance of `CYPHER_AST_REL_PATTERN` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_MAP` node, a `CYPHER_AST_PARAMETER` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_rel_pattern_get_properties(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_RANGE` node.
 *
 * @param [start] A `CYPHER_AST_INTEGER`, or null.
 * @param [end] A `CYPHER_AST_INTEGER`, or null.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_range(const cypher_astnode_t *start,
        const cypher_astnode_t *end, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range);

/**
 * Get the start of a `CYPHER_AST_RANGE` node.
 *
 * If the node is not an instance of `CYPHER_AST_RANGE` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_RANGE` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_range_get_start(
        const cypher_astnode_t *node);

/**
 * Get the end of a `CYPHER_AST_RANGE` node.
 *
 * If the node is not an instance of `CYPHER_AST_RANGE` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A `CYPHER_AST_RANGE` node, or null.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_range_get_end(
        const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_COMMAND` node.
 *
 * @param [name] A `CYPHER_AST_STRING` node.
 * @param [args] Arguments for the command, all of type `CYPHER_AST_STRING`.
 * @param [nargs] The number of clauses.
 * @param [children] The children of the node.
 * @param [nchildren] The number of children.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_command(const cypher_astnode_t *name,
        cypher_astnode_t * const *args, unsigned int nargs,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range);

/**
 * Get the name from a CYPHER_AST_COMMAND node.
 *
 * If the node is not an instance of `CYPHER_AST_COMMAND` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return A pointer to the name of the command, which will be of type
 *         `CYPHER_AST_STRING`.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_command_get_name(
        const cypher_astnode_t *node);

/**
 * Get the number of arguments from a CYPHER_AST_COMMAND node.
 *
 * If the node is not an instance of `CYPHER_AST_COMMAND` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @return The number of arguments to the command.
 */
__cypherlang_pure
unsigned int cypher_ast_command_narguments(const cypher_astnode_t *node);

/**
 * Get an argument from a CYPHER_AST_COMMAND node.
 *
 * If the node is not an instance of `CYPHER_AST_COMMAND` then the result will
 * be undefined.
 *
 * @param [node] The AST node.
 * @param [index] The index of the argument.
 * @return A pointer to the argument of the command, which will be of type
 *         `CYPHER_AST_STRING`, or `NULL` if there is no argument at the
 *         specified index.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_ast_command_get_argument(
        const cypher_astnode_t *node, unsigned int index);


/**
 * Construct a `CYPHER_AST_LINE_COMMENT` node.
 *
 * The node will also be an instance of `CYPHER_AST_COMMENT`.
 *
 * @param [s] A pointer to a character string containing the comment.
 * @param [n] The length of the character string.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_line_comment(const char *s, size_t n,
        struct cypher_input_range range);

/**
 * Get a null terminated string for a `CYPHER_AST_LINE_COMMENT` node.
 *
 * If the node is not an instance of `CYPHER_AST_LINE_COMMENT` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A pointer to the null terminated string.
 */
__cypherlang_pure
const char *cypher_ast_line_comment_get_value(const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_BLOCK_COMMENT` node.
 *
 * The node will also be an instance of `CYPHER_AST_COMMENT`.
 *
 * @param [s] A pointer to a character string containing the comment.
 * @param [n] The length of the character string.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_block_comment(const char *s, size_t n,
        struct cypher_input_range range);

/**
 * Get a null terminated string for a `CYPHER_AST_BLOCK_COMMENT` node.
 *
 * If the node is not an instance of `CYPHER_AST_BLOCK_COMMENT` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A pointer to the null terminated string.
 */
__cypherlang_pure
const char *cypher_ast_block_comment_get_value(const cypher_astnode_t *node);


/**
 * Construct a `CYPHER_AST_ERROR` node.
 *
 * @param [s] A pointer to a character string containing the erroneous input.
 * @param [n] The length of the character string.
 * @param [range] The input range.
 * @return An AST node, or NULL if an error occurs (errno will be set).
 */
__cypherlang_must_check
cypher_astnode_t *cypher_ast_error(const char *s, size_t n,
        struct cypher_input_range range);

/**
 * Get a null terminated string for a `CYPHER_AST_ERROR` node.
 *
 * If the node is not an instance of `CYPHER_AST_BLOCK_COMMENT` then the result
 * will be undefined.
 *
 * @param [node] The AST node.
 * @return A pointer to the null terminated string.
 */
__cypherlang_pure
const char *cypher_ast_error_get_value(const cypher_astnode_t *node);


/**
 * Release an entire AST tree.
 *
 * @param [ast] The root of the AST tree.
 */
void cypher_ast_free(cypher_astnode_t *ast);


#define CYPHER_AST_RENDER_DEFAULT 0

/**
 * Print a represetation of an AST to a stream.
 *
 * Useful for debugging purposes.
 *
 * @param [ast] The AST node.
 * @param [stream] The stream to print to.
 * @param [width] The width to render, which is advisory and may be exceeded.
 *         A value of 0 implies no restriction on width.
 * @param [colorization] A colorization scheme to apply, or null.
 * @param [flags] A bitmask of flags to control rendering.
 * @return 0 on success, or -1 if an error occurs (errno will be set).
 */
int cypher_ast_fprint(const cypher_astnode_t *ast, FILE *stream,
        unsigned int width,
        const struct cypher_parser_colorization *colorization,
        uint_fast32_t flags);


/*
 * =====================================
 * parser
 * =====================================
 */

/**
 * Configuration for a cypher parser.
 */
typedef struct cypher_parser_config cypher_parser_config_t;

/**
 * Generate a new parser configuration.
 *
 * The returned configuration must be later released using
 * cypher_parser_config_free().
 *
 * @return A pointer to a new parser configuration, or `NULL` if an error
 *         occurs (errno will be set).
 */
__cypherlang_must_check
cypher_parser_config_t *cypher_parser_new_config(void);

/**
 * Release a parser configuration.
 *
 * @param [config] A pointer to a parser configuration. This pointer will
 *         be invalid after the function returns.
 */
void cypher_parser_config_free(cypher_parser_config_t *config);

/**
 * Set the initial input position for parsing.
 *
 * By default, the initial position is line 1, column 1 (offset 0). By setting
 * a different position, all ranges in the AST, and positions in any error
 * messages, will be offset by the specified position - as if the input had
 * contained unparsed whitespace up until that point.
 *
 * @param [config] The parser configuration.
 * @param [position] The initial input position.
 */
void cypher_parser_config_set_initial_position(cypher_parser_config_t *config,
        struct cypher_input_position position);

/**
 * Set the initial ordinal for parsing.
 *
 * All AST nodes parsed will be numbered starting from the specified initial
 * ordinal, which is 0 by default. This affects the numbering of AST nodes
 * in rendering.
 *
 * @param [config] The parser configuration.
 * @param [n] The initial oridinal.
 */
void cypher_parser_config_set_initial_ordinal(cypher_parser_config_t *config,
        unsigned int n);


/**
 * Set the colorization scheme for error messages.
 *
 * @param [config] The parser configuration.
 * @param [colorization] The colorization scheme to use.
 */
void cypher_parser_config_set_error_colorization(cypher_parser_config_t *config,
        const struct cypher_parser_colorization *colorization);

/**
 * A parse segment.
 */
typedef struct cypher_parse_segment cypher_parse_segment_t;

/**
 * A parse result.
 */
typedef struct cypher_parse_result cypher_parse_result_t;

/**
 * A parse error.
 */
typedef struct cypher_parse_error cypher_parse_error_t;


/**
 * A parse callback.
 */
typedef int (*cypher_parser_segment_callback_t)(void *userdata,
        cypher_parse_segment_t *segment);


#define CYPHER_PARSE_DEFAULT 0
#define CYPHER_PARSE_SINGLE (1<<0)
#define CYPHER_PARSE_ONLY_STATEMENTS (1<<1)


/**
 * @fn int cypher_parse_each(const char *s, cypher_parser_segment_callback_t callback, void *userdata, struct cypher_input_position *last, cypher_parser_config_t *config, uint_fast32_t flags);
 * @brief Parse segments from a string.
 *
 * The provided callback is invoked for every segment of parsed input, where
 * each segments is separated by either a newline or semicolon (`;`),
 * respectively depending on whether a client command is being parsed or not.
 * If the flag CYPHER_PARSE_ONLY_STATEMENTS is set, then only semicolons will
 * be used for delimiting segments, and client commands will not be parsed.
 *
 * The segment will be released after the callback is complete, unless retained
 * using cypher_parse_segment_retain().
 *
 * @param [s] A null terminated string to parse.
 * @param [callback] The callback to be invoked for each parsed segment.
 * @param [userdata] A pointer that will be provided to the callback.
 * @param [last] Either `NULL`, or a pointer to a `struct cypher_input_position`
 *         that will be set position of the last character consumed from the
 *         input.
 * @param [config] Either `NULL`, or a pointer to configuration for the parser.
 * @param [flags] A bitmask of flags to control parsing.
 * @return 0 on success, -1 on failure (errno will be set).
 */
#define cypher_parse_each(s,b,d,l,c,f) (cypher_uparse_each(s,strlen(s),b,d,l,c,f))

/**
 * @fn cypher_parse_result_t *cypher_parse(const char *s, struct cypher_input_position *last, cypher_parser_config_t *config, uint_fast32_t flags);
 * @brief Parse a command or statement from a string.
 *
 * All statements and/or client commands are parsed from the input string, and
 * a result returned. The result must be passed to
 * cypher_parse_result_free() to release dynamically allocated memory.
 * If the flag CYPHER_PARSE_ONLY_STATEMENTS is set, client commands will not be
 * parsed.
 *
 * @param [s] A null terminated string to parse.
 * @param [last] Either `NULL`, or a pointer to a `struct cypher_input_position`
 *         that will be set position of the last character consumed from the
 *         input.
 * @param [config] Either `NULL`, or a pointer to configuration for the parser.
 * @param [flags] A bitmask of flags to control parsing.
 * @return A pointer to a `cypher_parse_result_t`, or `NULL` if an error occurs
 *         (errno will be set).
 */
#define cypher_parse(s,l,c,f) (cypher_uparse(s,strlen(s),l,c,f))

/**
 * Parse segments from a string.
 *
 * The provided callback is invoked for every segment of parsed input, where
 * each segments is separated by either a newline or semicolon (`;`),
 * respectively depending on whether a client command is being parsed or not.
 * If the flag CYPHER_PARSE_ONLY_STATEMENTS is set, then only semicolons will
 * be used for delimiting segments, and client commands will not be parsed.
 *
 * The segment will be released after the callback is complete, unless retained
 * using cypher_parse_segment_retain().
 *
 * @param [s] The string to parse.
 * @param [n] The size of the string.
 * @param [callback] The callback to be invoked for each parsed segment.
 * @param [userdata] A pointer that will be provided to the callback.
 * @param [last] Either `NULL`, or a pointer to a `struct cypher_input_position`
 *         that will be set position of the last character consumed from the
 *         input.
 * @param [config] Either `NULL`, or a pointer to configuration for the parser.
 * @param [flags] A bitmask of flags to control parsing.
 * @return A pointer to a `cypher_parse_result_t`, or `NULL` if an error occurs
 *         (errno will be set).
 */
__cypherlang_must_check
int cypher_uparse_each(const char *s, size_t n,
        cypher_parser_segment_callback_t callback, void *userdata,
        struct cypher_input_position *last, cypher_parser_config_t *config,
        uint_fast32_t flags);

/**
 * Parse a statement or command from a string.
 *
 * All statements and/or client commands are parsed from the input string, and
 * a result returned. The result must be passed to
 * cypher_parse_result_free() to release dynamically allocated memory.
 * If the flag CYPHER_PARSE_ONLY_STATEMENTS is set, client commands will not be
 * parsed.
 *
 * @param [s] The string to parse.
 * @param [n] The size of the string.
 * @param [last] Either `NULL`, or a pointer to a `struct cypher_input_position`
 *         that will be set position of the last character consumed from the
 *         input.
 * @param [config] Either `NULL`, or a pointer to configuration for the parser.
 * @param [flags] A bitmask of flags to control parsing.
 * @return A pointer to a `cypher_parse_result_t`, or `NULL` if an error occurs
 *         (errno will be set).
 */
__cypherlang_must_check
cypher_parse_result_t *cypher_uparse(const char *s, size_t n,
        struct cypher_input_position *last, cypher_parser_config_t *config,
        uint_fast32_t flags);

/**
 * Parse segments from a stream.
 *
 * The provided callback is invoked for every segment of parsed input, where
 * each segments is separated by either a newline or semicolon (`;`),
 * respectively depending on whether a client command is being parsed or not.
 * If the flag CYPHER_PARSE_ONLY_STATEMENTS is set, then only semicolons will
 * be used for delimiting segments, and client commands will not be parsed.
 *
 * The segment will be released after the callback is complete, unless retained
 * using cypher_parse_segment_retain().
 *
 * @param [stream] The stream to parse.
 * @param [callback] The callback to be invoked for each parsed segment.
 * @param [userdata] A pointer that will be provided to the callback.
 * @param [last] Either `NULL`, or a pointer to a `struct cypher_input_position`
 *         that will be set position of the last character consumed from the
 *         input.
 * @param [config] Either `NULL`, or a pointer to configuration for the parser.
 * @param [flags] A bitmask of flags to control parsing.
 * @return A pointer to a `cypher_parse_result_t`, or `NULL` if an error occurs
 *         (errno will be set).
 */
__cypherlang_must_check
int cypher_fparse_each(FILE *stream, cypher_parser_segment_callback_t callback,
        void *userdata, struct cypher_input_position *last,
        cypher_parser_config_t *config, uint_fast32_t flags);

/**
 * Parse a statement or command from a stream.
 *
 * All statements and/or client commands are parsed from the input string, and
 * a result returned. The result must be passed to
 * cypher_parse_result_free() to release dynamically allocated memory.
 * If the flag CYPHER_PARSE_ONLY_STATEMENTS is set, client commands will not be
 * parsed.
 *
 * @param [stream] The stream to parse.
 * @param [last] Either `NULL`, or a pointer to a `struct cypher_input_position`
 *         that will be set position of the last character consumed from the
 *         input.
 * @param [config] Either `NULL`, or a pointer to configuration for the parser.
 * @param [flags] A bitmask of flags to control parsing.
 * @return A pointer to a `cypher_parse_result_t`, or `NULL` if an error occurs
 *         (errno will be set).
 */
__cypherlang_must_check
cypher_parse_result_t *cypher_fparse(FILE *stream,
        struct cypher_input_position *last, cypher_parser_config_t *config,
        uint_fast32_t flags);


/**
 * Get the range of a parse segment.
 *
 * @param [segment] The parse segment.
 * @return The input range.
 */
__cypherlang_pure
struct cypher_input_range cypher_parse_segment_get_range(
        const cypher_parse_segment_t *segment);

/**
 * Get the number of errors encountered in a parse segment.
 *
 * @param [segment] The parse segment.
 * @return The number of errors.
 */
__cypherlang_pure
unsigned int cypher_parse_segment_nerrors(
        const cypher_parse_segment_t *segment);

/**
 * Get an error from a parse segment.
 *
 * @param [segment] The parse segment.
 * @param [index] The error index.
 * @return A pointer to the error description, or `NULL` if there is no error
 *         at the specified index.
 */
__cypherlang_pure
const cypher_parse_error_t *cypher_parse_segment_get_error(
        const cypher_parse_segment_t *segment, unsigned int index);

/**
 * Get the number of root AST nodes parsed in a segment.
 *
 * @param [segment] The parse segment.
 * @return The number of root AST nodes.
 */
__cypherlang_pure
unsigned int cypher_parse_segment_nroots(const cypher_parse_segment_t *segment);

/**
 * Get a root AST node from a parse segment.
 *
 * @param [segment] The parse segment.
 * @param [index] The node index.
 * @return A pointer to the AST node, or `NULL` if there is no node at the
 *         specified index.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_parse_segment_get_root(
        const cypher_parse_segment_t *segment, unsigned int index);

/**
 * Get the total number of AST nodes parsed in a segment.
 *
 * Includes all root nodes and their children, at any depth.
 *
 * @param [segment] The parse segment.
 * @return The number of elements.
 */
__cypherlang_pure
unsigned int cypher_parse_segment_nnodes(const cypher_parse_segment_t *segment);

/**
 * Get the statement or client command parsed in a segment.
 *
 * @param [segment] The parse segment.
 * @return Either a CYPHER_AST_STATEMENT, a CYPHER_AST_COMMAND node,
 *         or `NULL` if neither was encountered in the segment.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_parse_segment_get_directive(
        const cypher_parse_segment_t *segment);

/**
 * Check if the parse encountered the end of the input.
 *
 * Indicates if the segment was terminated by to the end of the input.
 *
 * @param [segment] The parse segment.
 * @return `true` if the end of input was encountered, `false` if the
 *         segment was terminated with an expected character.
 */
__cypherlang_pure
bool cypher_parse_segment_eof(const cypher_parse_segment_t *segment);

/**
 * Print a represetation of the AST from a parse segment to a stream.
 *
 * Useful for debugging purposes.
 *
 * @param [segment] The parse segment.
 * @param [stream] The stream to print to.
 * @param [width] The width to render, which is advisory and may be exceeded.
 *         A value of 0 implies no restriction on width.
 * @param [colorization] A colorization scheme to apply, or null.
 * @param [flags] A bitmask of flags to control rendering.
 * @return 0 on success, or -1 if an error occurs (errno will be set).
 */
int cypher_parse_segment_fprint_ast(const cypher_parse_segment_t *segment,
        FILE *stream, unsigned int width,
        const struct cypher_parser_colorization *colorization,
        uint_fast32_t flags);

/**
 * Retain a reference to a parse segment.
 *
 * The segment must later be passed to cypher_parse_segment_release().
 *
 * @param [segment] The parse segment.
 */
void cypher_parse_segment_retain(cypher_parse_segment_t *segment);


/**
 * Release a reference to a parse segment.
 *
 * The segment must have been previously retained using
 * cypher_parse_segment_retain().
 *
 * @param [segment] The parse segment.
 */
void cypher_parse_segment_release(cypher_parse_segment_t *segment);


/**
 * Get the number of root AST nodes parsed.
 *
 * @param [result] The parse result.
 * @return The number of root AST nodes.
 */
__cypherlang_pure
unsigned int cypher_parse_result_nroots(const cypher_parse_result_t *result);

/**
 * Get a root AST nodes from a parse result.
 *
 * @param [result] The parse result.
 * @param [index] The node index.
 * @return A pointer to the AST node, or `NULL` if there is no element at the
 *         specified index.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_parse_result_get_root(
        const cypher_parse_result_t *result, unsigned int index);

/**
 * Get the total number of AST nodes parsed.
 *
 * Includes all children, at any depth, of all result elements.
 *
 * @param [result] The parse result.
 * @return The number of elements.
 */
__cypherlang_pure
unsigned int cypher_parse_result_nnodes(const cypher_parse_result_t *result);

/**
 * Get the number of statements or commands parsed.
 *
 * @param [result] The parse result.
 * @return The number of statements or commands parsed.
 */
__cypherlang_pure
unsigned int cypher_parse_result_ndirectives(
        const cypher_parse_result_t *result);

/**
 * Get the AST for a parsed statement or command from a parse result.
 *
 * @param [result] The parse result.
 * @param [index] The directive index.
 * @return Either a CYPHER_AST_STATEMENT, a CYPHER_AST_COMMAND node,
 *         or `NULL` if there is no directive at the specified index.
 */
__cypherlang_pure
const cypher_astnode_t *cypher_parse_result_get_directive(
        const cypher_parse_result_t *result, unsigned int index);

/**
 * Get the number of errors encountered during parsing.
 *
 * @param [result] The parse result.
 * @return The number of errors.
 */
__cypherlang_pure
unsigned int cypher_parse_result_nerrors(const cypher_parse_result_t *result);

/**
 * Get an error description from a parse result.
 *
 * @param [result] The parse result.
 * @param [index] The error index.
 * @return A pointer to the error description, or `NULL` if there is no value
 *         at the specified index.
 */
__cypherlang_pure
const cypher_parse_error_t *cypher_parse_result_get_error(
        const cypher_parse_result_t *result, unsigned int index);

/**
 * Check if the parse encountered the end of the input.
 *
 * Indicates if the last parsed command or statement was terminated
 * (with a newline or `;` respectively), or with EOF.
 *
 * @param [result] The parse result.
 * @return `true` if the end of input was encountered, `false` if the
 *         command or statement was terminated with the expected character.
 */
__cypherlang_pure
bool cypher_parse_result_eof(const cypher_parse_result_t *result);


/**
 * Print a represetation of a parse result to a stream.
 *
 * Useful for debugging purposes.
 *
 * @param [result] The parse result.
 * @param [stream] The stream to print to.
 * @param [width] The width to render, which is advisory and may be exceeded.
 *         A value of 0 implies no restriction on width.
 * @param [colorization] A colorization scheme to apply, or null.
 * @param [flags] A bitmask of flags to control rendering.
 * @return 0 on success, or -1 if an error occurs (errno will be set).
 */
int cypher_parse_result_fprint_ast(const cypher_parse_result_t *result,
        FILE *stream, unsigned int width,
        const struct cypher_parser_colorization *colorization,
        uint_fast32_t flags);

/**
 * Free memory associated with a parse result.
 *
 * The result will no longer be valid after this function is invoked.
 *
 * @param [result] The parse result.
 */
void cypher_parse_result_free(cypher_parse_result_t *result);


/**
 * Get the position of an error.
 *
 * @param [error] The parse error.
 * @return The input position.
 */
__cypherlang_pure
struct cypher_input_position cypher_parse_error_position(
        const cypher_parse_error_t *error);

/**
 * Get the error message of an error.
 *
 * @param [error] The parse error.
 * @return The message.
 */
__cypherlang_pure
const char *cypher_parse_error_message(const cypher_parse_error_t *error);

/**
 * Get the error context of an error.
 *
 * This returns a pointer to a null-terminated string, which contains a
 * section of the input around where the error occurred, that is limited
 * in length and suitable for presentation to a user.
 *
 * @param [error] The parse error.
 * @return The context string.
 */
__cypherlang_pure
const char *cypher_parse_error_context(const cypher_parse_error_t *error);

/**
 * Get the offset into the context of an error.
 *
 * Identifies the point of the error within the context string, allowing
 * this to be reported to the user, typically with an arrow pointing to the
 * invalid character.
 *
 * @param [error] The parse error.
 * @return The offset into the errors context string.
 */
__cypherlang_pure
size_t cypher_parse_error_context_offset(const cypher_parse_error_t *error);


/*
 * ====================================
 * quick parser
 * ====================================
 */

/**
 * A quick parse segment.
 */
typedef struct cypher_quick_parse_segment cypher_quick_parse_segment_t;


/**
 * A quick parse callback.
 */
typedef int (*cypher_parser_quick_segment_callback_t)(void *userdata,
        const cypher_quick_parse_segment_t *segment);

/**
 * @fn int cypher_quick_parse(const char *s, cypher_parser_quick_segment_callback_t callback, void *userdata, uint_fast32_t flags);
 * @brief Quick parse segments from a string.
 *
 * The provided callback is invoked for every segment of parsed input, where
 * each segments is separated by either a newline or semicolon (`;`),
 * respectively depending on whether a client command is being parsed or not.
 * If the flag CYPHER_PARSE_ONLY_STATEMENTS is set, then only semicolons will
 * be used for delimiting segments, and client commands will not be parsed.
 *
 * @param [s] The string to parse.
 * @param [callback] The callback to be invoked for each parsed segment.
 * @param [userdata] A pointer that will be provided to the callback.
 * @param [flags] A bitmask of flags to control parsing.
 * @return 0 on success, or -1 on failure (errno will be set).
 */
#define cypher_quick_parse(s,c,u,f) (cypher_quick_uparse(s,strlen(s),c,u,f))

/**
 * Quick parse segments from a string.
 *
 * The provided callback is invoked for every segment of parsed input, where
 * each segments is separated by either a newline or semicolon (`;`),
 * respectively depending on whether a client command is being parsed or not.
 * If the flag CYPHER_PARSE_ONLY_STATEMENTS is set, then only semicolons will
 * be used for delimiting segments, and client commands will not be parsed.
 *
 * @param [s] The string to parse.
 * @param [n] The size of the string.
 * @param [callback] The callback to be invoked for each parsed segment.
 * @param [userdata] A pointer that will be provided to the callback.
 * @param [flags] A bitmask of flags to control parsing.
 * @return 0 on success, or -1 on failure (errno will be set).
 */
__cypherlang_must_check
int cypher_quick_uparse(const char *s, size_t n,
        cypher_parser_quick_segment_callback_t callback, void *userdata,
        uint_fast32_t flags);

/**
 * Quick parse a statement or command from a stream.
 *
 * The provided callback is invoked for every segment of parsed input, where
 * each segments is separated by either a newline or semicolon (`;`),
 * respectively depending on whether a client command is being parsed or not.
 * If the flag CYPHER_PARSE_ONLY_STATEMENTS is set, then only semicolons will
 * be used for delimiting segments, and client commands will not be parsed.
 *
 * @param [stream] The stream to parse.
 * @param [callback] The callback to be invoked for each parsed segment.
 * @param [userdata] A pointer that will be provided to the callback.
 * @param [flags] A bitmask of flags to control parsing.
 * @return 0 on success, or -1 on failure (errno will be set).
 */
int cypher_quick_fparse(FILE *stream,
        cypher_parser_quick_segment_callback_t callback, void *userdata,
        uint_fast32_t flags);

/**
 * Get a pointer to the start of the segment string.
 *
 * @param [segment] The quick parse segment.
 * @return A pointer to a character string.
 */
__cypherlang_pure
const char *cypher_quick_parse_segment_get_ptr(
        const cypher_quick_parse_segment_t *segment);

/**
 * Get the length of the segment string.
 *
 * @param [segment] The quick parse segment.
 * @return The length of the string (in bytes).
 */
__cypherlang_pure
size_t cypher_quick_parse_segment_get_length(
        const cypher_quick_parse_segment_t *segment);

/**
 * Get the range of a quick parse segment.
 *
 * @param [segment] The quick parse segment.
 * @return The input range.
 */
__cypherlang_pure
struct cypher_input_range cypher_quick_parse_segment_get_range(
        const cypher_quick_parse_segment_t *segment);

/**
 * Check if the quick parse encountered the end of the input.
 *
 * Indicates if the segment was terminated by to the end of the input.
 *
 * @param [segment] The quick parse segment.
 * @return `true` if the end of input was encountered, `false` if the
 *         segment was terminated with an expected character.
 */
__cypherlang_pure
bool cypher_quick_parse_segment_eof(
        const cypher_quick_parse_segment_t *segment);


#pragma GCC visibility pop

#ifdef __cplusplus
}
#endif

#endif/*CYPHER_PARSER_H*/
