#~ Copyright 2010 Jonathan Nelisse.
#~ Distributed under the Boost Software License, Version 1.0.
#~ (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

# This file contains tables that are used to generate classes and traversal functions
# for these classes. A prerequisite is that each class has a corresponding ATerm
# representation (the first column of each table). The second column contains the
# constructor of the classes. If the name of the class has a postfix between brackets
# like variable[_base] then the generated class will be called variable_base, but the
# traversal will use variable. This is done to enable the user of the class to add
# additional behavior to the base class.

import re
import string

NUMERIC_EXPRESSION_CLASSES = r'''
Common          | common(const common_expression& operand)                                | A common expression
Number          | number(const number_expression& number)                                 | A number
Card            | card(const set_expression& set)                                         | The cardinality of a set
Length          | length(const seq_expression& seq)                                       | The length of a sequence
Plus            | plus(const numeric_expression& left, const numeric_expression& right)   | An addition
Minus           | minus(const numeric_expression& left, const numeric_expression& right)  | A subtraction
Times           | times(const numeric_expression& left, const numeric_expression& right)  | A multiplication
Div             | div(const numeric_expression& left, const numeric_expression& right)    | A division
Mod             | mod(const numeric_expression& left, const numeric_expression& right)    | A modulo operation
Min             | min(const numeric_expression& operand)                                  | A negative number
'''                                                                                                                                 

BOOLEAN_EXPRESSION_CLASSES = r'''
Common			| common(const common_expression& operand)								  | A common expression
true            | true_()							 									  | The true value
false           | false_()																  | The false value
And				| and_(const boolean_expression& left, const boolean_expression& right)	  | An and
Or				| or_(const boolean_expression& left, const boolean_expression& right)	  | An or
Not             | not(const boolean_expression& operand)								  | An not
Null			| null(const seq_expression& seq)										  | The empty sequence test
Elem			| elem(const expression& expr, const seq_expression& seq)				  | The membership of a sequence test
Member			| member(const expression& expr, set_expression& set)					  | The membership of a set test
Empty			| empty(const set_expression& set)										  | The empty set test
Equal			| equal(const expression& left, const expression& right)				  | An equality test
NotEqual		| notequal(const expression& left, const expression& right)				  | An inequality test
Less			| less(const expression& left, const expression& right)					  | A less test
LessOrEqual		| lessorequal(const expression& left, const expression& right)			  | A less or equal test
Greater			| greater(const expression& left, const expression& right)				  | A greater test
GreaterOrEqual	| greaterorequal(const expression& left, const expression& right)		  | A greater or equal test
'''

SET_EXPRESSION_CLASSES = r'''
Common			| common(const common_expression& operand)									| A common expression
Targ			| targ(const targ& targ)													| A simple argument
TargGens		| targgens(const targ& targ, const generator_list& gens)					| A complex argument
Targ0			| targ0(const targ& targ)													| A simple argument (for channels)
TargGens0		| targgens0(const targ& targ, const generator_list& gens)					| A complex argument (for channels)
union			| union(const set_expression& left, const set_expression& right)			| A union
inter			| inter(const set_expression& left, const set_expression& right)			| An intersection
diff			| diff(const set_expression& left, const set_expression& right)				| A difference
Union			| union(const set_expression& set)											| A set union
Inter			| inter(const set_expression& set)											| A set intersection
set				| set(const seq_expression& seq)											| The set of a sequence
Set				| set(const set_expression& set)											| The set of sets of a set
Seq				| seq(const seq_expression& seq)											| The set of sequences of a sequence
extensions		| extensions(const expression& expr)										| The extension set
productions		| productions(const expression& expr)										| The production set
'''

SEQ_EXPRESSION_CLASSES = r'''
Common			| common(const common_expression& operand)									| A common expression
Targ			| targ(const targ& targ)													| A simple argument
TargGens		| targgens(const targ& targ, const generator_list& gens)					| A complex argument
Cat				| cat(const seq_expression& left, const seq_expression& right)				| The concatenation of two sequences
Concat			| concat(const seq_expression& seq)											| The concatenation of a sequence of sequences
Head			| head(const seq_expression& seq)											| The head of a non-empty sequence
Tail			| tail(const seq_expression& seq)											| The tail of a non-empty sequence
'''

TUPLE_EXPRESSION_CLASSES = r'''
Common			| common(const common_expression& operand)									| A common expression
Exprs			| exprs(const expression_list& exprs)										| A tuple expression
'''

DOTTED_EXPRESSION_CLASSES = r'''
Common			| common(const common_expression& operand)									| A common expression
Dot				| dot(const expression& left, expression& right)							| A dotted expression
'''

LAMBDA_EXPRESSION_CLASSES = r'''
Common			| common(const common_expression& operand)									| A common expression
LambdaExpr		| lambdaexpr(const expression_list& exprs, const any& any)					| A lambda expression
'''

COMMON_EXPRESSION_CLASSES = r'''
Conditional		| conditional(const boolean_expression& guard, const any& thenpart, const any& elsepart)	| A conditional
Name			| name(const name& name)													| A name
LambdaAppl		| lambdaappl(const lambda_expression& lambda, const expression_list& exprs)	| A lambda application
LocalDef		| localdef(const definition_list& defs, const any& any)						| A local definition
Bracketed		| bracketed(const any& operand)												| A bracketed process or expression
'''

EXPRESSION_CLASSES = r'''
Numb			| numb(const numeric_expression& operand)									| A numeric expression
Bool			| bool_(const boolean_expression& operand)									| A boolean expression
Set				| set(const set_expression& operand)										| A set expression
Seq				| seq(const seq_expression& operand)										| A seq expression
Tuple			| tuple(const tuple_expression& operand)									| A tuple expression
Dotted			| dotted(const dotted_expression& operand)									| A dotted expression
Lambda			| lambda(const lambda_expression& operand)									| A lambda expression
'''

ANY_CLASSES = r'''
Expr			| expr(const expression& operand)											| An expression
Proc			| proc(const process& operand)												| A process
'''

DEFINITION_CLASSES = r'''
Assign			| assign(const any& left, const any& right)									| An assignment
Channel			| channel(const name_list& names, const type& type)							| A channel
SimpleChannel	| channel(const name_list& names)											| A simple channel
NameType		| nametype(const name& name, const type& type)								| A nametype
DataType		| datatype(const name& name, const vartype_list& vartypes)					| A datatype
SubType			| subtype(const name& name, const vartype_list& vartypes)					| A subtype
External		| external(const name_list& names)											| An external
Transparent		| transparent(const trname_list& trnames)									| A transparent
Assert			| assert_(const check& check)												| An assertion
Print			| print(const expression& expr)												| A print
Include			| include(const filename& filename)											| An include
'''

VARTYPE_CLASSES = r'''
SimpleBranch	| simplebranch(const name& name)											| A simple branch
Branch			| branch(const name& name, const type& type)								| A branch
'''

TYPE_CLASSES = r'''
TypeProduct		| typeproduct(const type& left, const type& right)							| A type product
TypeTuple		| typetuple(const type_list& types)											| A type tuple
TypeSet			| typeset(const set_expression& set)										| A type set
SympleTypeName	| simpletypename(const name& name)											| A simple type name
TypeName		| typename(const name& name, const type& type)								| A type name
'''

CHECK_CLASSES = r'''
BCheck			| bcheck(const boolean_expression& expr)									| A boolean check
RCheck			| rcheck(const process& left, const process& right, const refined& refined)	| A refinement check
TCheck			| tcheck(const process& process, const test& test)							| A test
NotCheck		| notcheck(const check& check)												| A negated check
'''

REFINED_CLASSES = r'''
Model			| model(const model& model)													| A model
'''

MODEL_CLASSES = r'''
Nil				| nil()																		| An empty
T				| t()																		| A traces
'''

FAILUREMODEL_CLASSES = r'''
F				| f()																		| A failures
FD				| fd()																		| A faulures/divergences
'''

TEST_CLASSES = r'''
divergence_free	| divergence_free()															| A divergence free
Test			| test(const testtype& testtype, const failuremodel& failuremodel)			| A complex test
'''

TESTTYPE_CLASSES = r'''
deterministic	| deterministic()															| A deterministic
deadlock_free	| deadlock_free()															| A deadlock free
livelock_free	| livelock_free()															| A livelock free
'''

TRNAME_CLASSES = r'''
normal			| normal()																	| A normal
normalise		| normalise()																| A normal
normalize		| normalize()																| A normal
sbsim			| sbsim()																	| A sbsim
tau_loop_factor	| tau_loop_factor()															| A tau_loop_factor
diamond			| diamond()																	| A diamond
model_compress	| model_compress()															| A model compress
explicate		| explicate()																| An explicate
'''

FILENAME_CLASSES = r'''
FileName		| filename(const name_list& names)											| A filename
'''

FDRSPEC_CLASSES = r'''
FDRSpec			| fdrspec(const definition_list& defs)										| An FDR specification
'''

TARG_CLASSES = r'''
Nil				| nil()																		| An empty
Exprs			| exprs(const expression_list& exprs)										| An expression list
ClosedRange		| closedrange(const numeric_expression& begin, const numeric_expression& end)	| A closed range
OpenRange		| openrange(const numeric_expression& begin)									| An open range
'''

GEN_CLASSES = r'''
BGen			| bgen(const boolean_expression& operand)									| A boolean
Gen				| gen(const expression& left, const expression& right)						| A generator
'''

PROCESS_CLASSES = r'''
Common			| common(const common_expression& operand)									| A common expression
STOP			| stop()																	| A stop
SKIP			| skip()																	| A skip
CHAOS			| chaos(const set_expression& set)											| A chaos
Prefix			| prefix(const dotted_expression& dotted, const field_list& fields, const process& process)	| A prefix
ExternalChoice	| externalchoice(const process& left, const process& right)					| An external choice
InternalChoice	| internalchoice(const process& left, const process& right)					| An internal choice
SequentialComposition	| sequentialcomposition(const process& left, const process& right)	| A sequential composition
Interrupt		| interrupt(const process& left, const process& right)						| An interrupt
Hiding			| hiding(const process& process, const set_expression& set)					| An hiding
Rename			| rename(const process& process, const renaming& renaming)					| A renaming
Interleave		| interleave(const process& left, const process& right)						| An interleave
Sharing			| sharing(const process& left, const process& right, const set_expression& set)	| A sharing
AlphaParallel	| alphaparallel(const process& left, const process& right, const set_expression& left_set, const set_expression& right_set)	| An alpha parallel
RepExternalChoice	| repexternalchoice(const generator_list& gens, const process& process)	| A replicated external choice
RepInternalChoice	| repinternalchoice(const generator_list& gens, const process& process)	| A replicated internal choice
RepSequentialComposition	| repsequentialcomposition(const generator_list& gens, const process& process)	| A replicated sequential composition
RepInterleave	| repinterleave(const generator_list& gens, const process& process)			| A replicated interleave
RepSharing		| repsharing(const generator_list& gens, const process& process, const set_expression& set)	| A replicated sharing
RepAlphaParallel	| repalphaparallel(const generator_list& gens, const process& process, const set_expression& set)	| A replicated alpha parallel
UntimedTimeOut	| untimedtimeout(const process& left, const process& right)					| An untimed time-out
BoolGuard		| boolguard(const boolean_expression& guard, const process& process)		| A boolean guard
LinkedParallel	| linkedparallel(const process& left, const process& right, const linkpar& linkpar)	| A linked parallel
RepLinkedParallel	| replinkedparallel(const generator_list& gens, const process& process, const linkpar& linkpar)	| A replicated linked parallel
'''

FIELD_CLASSES = r'''
SimpleInput		| simpleinput(const expression& expr)										| A simple input
Input			| input(const expression& expr, const set_expression& restriction)			| An input
Output			| output(const expression& expr)											| An output
'''

RENAMING_CLASSES = r'''
Maps			| maps(const map_list& maps)												| A map list
MapsGens		| mapsgens(const map_list& maps, const generator_list& gens)				| A map/generator list
'''

MAP_CLASSES = r'''
Map				| map(const dotted_expression& left, const dotted_expression& right)		| A map
'''

LINKPAR_CLASSES = r'''
Links			| links(const link_list& links)												| A link list
LinksGens		| linksgens(const link_list& links, const generator_list& gens)				| A link/generator list
'''

LINK_CLASSES = r'''
Link			| link(const dotted_expression& left, const dotted_expression& right)		| A link
'''
