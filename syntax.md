# 	Project 2

1. $`\text{Start} \rightarrow \text{ExprList}`$
2. $`\text{ExprList} \rightarrow \text{ExprList}\ \textbf{;}\ \text{Expression}\ |\ \text{Expression}`$
20. $`\text{Expression} \rightarrow \text{Additive-Expression}\ \textbf{?}\ \text{Expression}\ \textbf{:}\ \text{Expression}\ |\ \text{Additive-Expression}`$
4. $`\text{Additive-Expression} \rightarrow \text{Additive-Expression}\ \text{addop}\ \text{Term}\ |\ \text{Term}`$
5. $`\text{addop} \rightarrow \textbf{+}\ |\ \textbf{-}`$
6. $`\text{Term} \rightarrow \text{Term}\ \text{mulop}\ \text{Factor}\ |\ \text{Factor}`$
7. $`\text{mulop} \rightarrow \textbf{*}\ |\ \textbf{/}`$
8. $`\text{Factor} \rightarrow \textbf{(}\ \text{Expression}\ \textbf{)}\ |\ \textbf{INTEGER}`$
