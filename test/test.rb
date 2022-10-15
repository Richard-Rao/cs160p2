require_relative 'testlib'

prepare do
	executable "./oracle"
	testing "./calculator"


	to :scan, "-s"
	to :parse, ""
	to :eval, "-e"
end

doing :scan do
	pass "-1", "- Integer1"
	pass "2+4*3+33", "Integer2 + Integer4 * Integer3 + Integer33"
	pass "1 ? 1 : 1", "Integer1 ? Integer1 : Integer1"
	pass "1 ; 1 \n ; 1", "Integer1 ; Integer1 EOL ; Integer1"
	pass "3+2()", "Integer3 + Integer2 ( )"
	pass "3++", "Integer3 + +"
	fail "r+3", "Scan error: found invalid character r at line 1"
	fail "3+2a", "Scan error: found invalid character a at line 1"
	fail "4^2+2", "Scan error: found invalid character ^ at line 1"
	fail "5med2","Scan error: found invalid character m at line 1"
end

doing :parse do
	pass "2+4*((3+3))", ""
	pass "1 ? 100 : 2 * 3", ""
	pass "3+\n2; \n5+4", ""
	fail "1 + ( 2 * \n3 );", "Parse error: found invalid token EOF at line 2"
	fail "1\n2", "Parse error: found invalid token Integer at line 2"
	fail "-1*3", "Parse error: found invalid token - at line 1"
	fail "3123 * 5 - ) 321", "Parse error: found invalid token ) at line 1"
	fail "()", "Parse error: found invalid token ) at line 1"
	fail "(1+\n2", "Parse error: expected ) found EOF at line 2"
	fail "3++", "Parse error: found invalid token + at line 1"
end

doing :eval do
	pass "001 ; 02 ; 0003", "1\n2\n3"
	pass "22*2-(9*5-5)", "4"
	pass "24 / (3 + 4 - 1) * 4 \n * 6 / 4 / 6", "4"
	pass "24 / 5" , "4"
	pass "0 ? 1 : 2", "2"
	pass "1 ? 1 ? 2 : 3 : 0 ? 4 : 5", "2"
	pass "1 ? 1 ? 4 * 6 \n : 3 * 10 : 1 - 1 ? 3 - 7 : 5", "24"
	fail "1 + ( 2 * \n3 );", "Parse error: found invalid token EOF at line 2"
	fail "1534893*5555550", "Semantic error: number 8527174806150 out of bounds at line 1"
	fail "1/2;\n3/0", "Semantic error: dividing 3 with zero at line 2"
end

test!
