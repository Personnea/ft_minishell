#!/bin/bash
# =============================================================================
# Comprehensive test suite for ft_minishell
# Tests general shell behavior and heredoc behavior extensively
# =============================================================================

MINISHELL="./minishell"
PASS=0
FAIL=0
TMPDIR_TEST=$(mktemp -d)

RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
RESET='\033[0m'

# run_test NAME INPUT EXPECTED [OUTFILE]
# Sends INPUT + newline + "exit" to minishell.
# If OUTFILE given, checks file content; otherwise checks minishell stdout+stderr.
# PASS if EXPECTED is a substring of actual output.
run_test() {
    local name="$1"
    local input="$2"
    local expected="$3"
    local outfile="${4:-}"
    local ms_out
    local actual

    ms_out=$(printf '%s\nexit\n' "$input" | "$MINISHELL" 2>&1)

    if [ -n "$outfile" ]; then
        actual=$(cat "$outfile" 2>/dev/null)
    else
        actual="$ms_out"
    fi

    if echo "$actual" | grep -qF "$expected"; then
        printf "${GREEN}[PASS]${RESET} %s\n" "$name"
        PASS=$((PASS + 1))
    else
        printf "${RED}[FAIL]${RESET} %s\n" "$name"
        printf "       expected: |%s|\n" "$expected"
        printf "       actual:   |%s|\n" "$actual"
        FAIL=$((FAIL + 1))
    fi
}

# run_test_absent NAME INPUT ABSENT [OUTFILE]
# Sends INPUT + newline + "exit" to minishell.
# If OUTFILE given, checks file content; otherwise checks minishell stdout+stderr.
# PASS if ABSENT does NOT appear anywhere in the actual output.
run_test_absent() {
    local name="$1"
    local input="$2"
    local absent="$3"
    local outfile="${4:-}"
    local ms_out
    local actual

    ms_out=$(printf '%s\nexit\n' "$input" | "$MINISHELL" 2>&1)

    if [ -n "$outfile" ]; then
        actual=$(cat "$outfile" 2>/dev/null)
    else
        actual="$ms_out"
    fi

    if echo "$actual" | grep -qF "$absent"; then
        printf "${RED}[FAIL]${RESET} %s  (unexpected '%s' found)\n" "$name" "$absent"
        FAIL=$((FAIL + 1))
    else
        printf "${GREEN}[PASS]${RESET} %s\n" "$name"
        PASS=$((PASS + 1))
    fi
}

# run_test_exact NAME INPUT EXPECTED_CONTENT OUTFILE
# Runs INPUT, then checks that OUTFILE content matches EXPECTED_CONTENT exactly
# (modulo trailing newline stripped by $()).
run_test_exact() {
    local name="$1"
    local input="$2"
    local expected="$3"
    local outfile="$4"

    printf '%s\nexit\n' "$input" | "$MINISHELL" >/dev/null 2>&1
    local actual
    actual=$(cat "$outfile" 2>/dev/null)

    if [ "$actual" = "$expected" ]; then
        printf "${GREEN}[PASS]${RESET} %s\n" "$name"
        PASS=$((PASS + 1))
    else
        printf "${RED}[FAIL]${RESET} %s\n" "$name"
        printf "       expected: |%s|\n" "$expected"
        printf "       actual:   |%s|\n" "$actual"
        FAIL=$((FAIL + 1))
    fi
}

section() {
    printf "\n${CYAN}=== %s ===${RESET}\n" "$1"
}

# =============================================================================
# SECTION: Built-in commands
# =============================================================================
section "Built-in: echo"

run_test "echo basic" \
    "echo hello" \
    "hello"

run_test "echo multiple args" \
    "echo foo bar baz" \
    "foo bar baz"

run_test "echo -n suppresses newline" \
    "echo -n hello" \
    "hello"

run_test "echo empty" \
    "echo" \
    ""

run_test "echo with single quotes" \
    "echo 'hello world'" \
    "hello world"

run_test "echo with double quotes" \
    'echo "hello world"' \
    "hello world"

run_test "echo with escaped dollar in single quotes" \
    "echo '\$MY_ECHO_VAR'" \
    '$MY_ECHO_VAR'

run_test "echo variable expansion" \
    $'export GREET=world\necho hello $GREET' \
    "hello world"

section "Built-in: pwd"

run_test "pwd outputs current directory" \
    "pwd" \
    "/"

section "Built-in: cd"

run_test "cd to root and pwd" \
    $'cd /\npwd' \
    "/"

run_test "cd to /tmp and pwd" \
    $'cd /tmp\npwd' \
    "/tmp"

run_test "cd to nonexistent dir does not crash" \
    "cd /nonexistent_dir_12345" \
    ""

section "Built-in: export / env"

run_test "export sets variable" \
    $'export MY_VAR=hello42\necho $MY_VAR' \
    "hello42"

run_test "export variable visible in env" \
    $'export MY_ENV_VAR=42test\nenv' \
    "MY_ENV_VAR=42test"

run_test "export overwrite" \
    $'export A=first\nexport A=second\necho $A' \
    "second"

section "Built-in: unset"

run_test "unset removes variable echo" \
    $'export DEL_ME=here\nunset DEL_ME\necho $DEL_ME' \
    ""

run_test_absent "unset: variable no longer in env" \
    $'export DEL_VAR=yes\nunset DEL_VAR\nenv > '"$TMPDIR_TEST"'/unset_env.txt' \
    "DEL_VAR=yes" \
    "$TMPDIR_TEST/unset_env.txt"

section "Built-in: exit"

run_test "exit with no args" \
    "exit" \
    ""

run_test "exit code via \$? after false" \
    $'false\necho $?' \
    "1"

run_test "exit code 0 after true" \
    $'true\necho $?' \
    "0"

run_test "exit code 127 after command not found" \
    $'nonexistent_cmd_xyz\necho $?' \
    "127"

# =============================================================================
# SECTION: Redirections
# =============================================================================
section "Redirections: output >"

OF1="$TMPDIR_TEST/out1.txt"
run_test_exact "redir_output creates and writes file" \
    "echo hello_redir > $OF1" \
    "hello_redir" \
    "$OF1"

OF2="$TMPDIR_TEST/out2.txt"
redir_overwrite_input="echo first > $OF2
echo second > $OF2"
run_test_exact "redir_output overwrites existing file" \
    "$redir_overwrite_input" \
    "second" \
    "$OF2"

section "Redirections: append >>"

OF3="$TMPDIR_TEST/out3.txt"
echo "line1" > "$OF3"
run_test "redir_append keeps old content" \
    "echo line2 >> $OF3" \
    "line1" \
    "$OF3"

run_test "redir_append adds new line" \
    "echo line2 >> $OF3" \
    "line2" \
    "$OF3"

section "Redirections: input <"

echo "input_file_content" > "$TMPDIR_TEST/input.txt"
run_test "redir_input reads from file" \
    "cat < $TMPDIR_TEST/input.txt" \
    "input_file_content"

# =============================================================================
# SECTION: Pipes
# =============================================================================
section "Pipes"

run_test "simple pipe echo | cat" \
    "echo piped_output | cat" \
    "piped_output"

run_test "pipe with grep" \
    "echo hello | grep hello" \
    "hello"

run_test "pipe count lines" \
    "printf 'a\nb\nc\n' | wc -l" \
    "3"

run_test "multi-pipe" \
    "echo hello | cat | cat | cat" \
    "hello"

# =============================================================================
# SECTION: Variable expansion
# =============================================================================
section "Variable expansion"

run_test "expand undefined var is empty" \
    "echo \$UNDEFINED_VAR_XYZ" \
    ""

run_test "expand \$? after false" \
    $'false\necho $?' \
    "1"

run_test "expand \$? after true" \
    $'true\necho $?' \
    "0"

run_test "expand \$? chained" \
    $'false\necho $?\ntrue\necho $?' \
    "0"

# =============================================================================
# SECTION: Heredoc - basic behavior
# =============================================================================
section "Heredoc: basic"

HD1="$TMPDIR_TEST/hd1.txt"
run_test_exact "heredoc basic single line" \
    "cat <<EOF > $HD1
hello_heredoc
EOF" \
    "hello_heredoc" \
    "$HD1"

HD2="$TMPDIR_TEST/hd2.txt"
run_test_exact "heredoc multiline content" \
    "cat <<EOF > $HD2
line1
line2
line3
EOF" \
    "line1
line2
line3" \
    "$HD2"

HD3="$TMPDIR_TEST/hd3.txt"
run_test_exact "heredoc empty content (delimiter immediately)" \
    "cat <<EOF > $HD3
EOF" \
    "" \
    "$HD3"

HD_ALPHA="$TMPDIR_TEST/hd_alpha.txt"
run_test_exact "heredoc with non-standard delimiter STOP" \
    "cat <<STOP > $HD_ALPHA
alpha
STOP" \
    "alpha" \
    "$HD_ALPHA"

HD_LONG="$TMPDIR_TEST/hd_long.txt"
run_test_exact "heredoc with long delimiter ENDOFFILE" \
    "cat <<ENDOFFILE > $HD_LONG
content
ENDOFFILE" \
    "content" \
    "$HD_LONG"

# =============================================================================
# SECTION: Heredoc - variable expansion
# =============================================================================
section "Heredoc: variable expansion"

HD4="$TMPDIR_TEST/hd4.txt"
run_test_exact "heredoc expands \$VAR" \
    "export HD_VAR=expanded_value
cat <<EOF > $HD4
\$HD_VAR
EOF" \
    "expanded_value" \
    "$HD4"

HD5="$TMPDIR_TEST/hd5.txt"
run_test_exact "heredoc expands undefined var to empty" \
    "cat <<EOF > $HD5
before__after
EOF" \
    "before__after" \
    "$HD5"

HD7="$TMPDIR_TEST/hd7.txt"
run_test_exact "heredoc expands \$? after false (exit status 1)" \
    "false
cat <<EOF > $HD7
status=\$?
EOF" \
    "status=1" \
    "$HD7"

HD8="$TMPDIR_TEST/hd8.txt"
run_test_exact "heredoc expands \$? after true (exit status 0)" \
    "true
cat <<EOF > $HD8
status=\$?
EOF" \
    "status=0" \
    "$HD8"

HD9="$TMPDIR_TEST/hd9.txt"
run_test_exact "heredoc multiple vars on different lines" \
    "export A=foo
export B=bar
cat <<EOF > $HD9
\$A
\$B
EOF" \
    "foo
bar" \
    "$HD9"

# =============================================================================
# SECTION: Heredoc - quoted delimiter (no expansion)
# =============================================================================
section "Heredoc: quoted delimiter disables expansion"

HD10="$TMPDIR_TEST/hd10.txt"
run_test_exact "heredoc single-quoted delimiter: no expansion" \
    "export HD_NEXP=secret
cat <<'EOF' > $HD10
\$HD_NEXP
EOF" \
    '$HD_NEXP' \
    "$HD10"

HD11="$TMPDIR_TEST/hd11.txt"
run_test_exact 'heredoc double-quoted delimiter: no expansion' \
    "export HD_NEXP2=secret2
cat <<\"EOF\" > $HD11
\$HD_NEXP2
EOF" \
    '$HD_NEXP2' \
    "$HD11"

HD12="$TMPDIR_TEST/hd12.txt"
run_test_exact "heredoc single-quoted delimiter: \$? not expanded" \
    "false
cat <<'DELIM' > $HD12
exit=\$?
DELIM" \
    'exit=$?' \
    "$HD12"

# =============================================================================
# SECTION: Heredoc - interaction with pipes
# =============================================================================
section "Heredoc: pipes"

HD13="$TMPDIR_TEST/hd13.txt"
run_test_exact "heredoc piped to wc -l" \
    "cat <<EOF | wc -l > $HD13
a
b
c
EOF" \
    "3" \
    "$HD13"

HD14="$TMPDIR_TEST/hd14.txt"
run_test_exact "heredoc piped to grep" \
    "cat <<EOF | grep hello > $HD14
hello world
goodbye world
EOF" \
    "hello world" \
    "$HD14"

HD15="$TMPDIR_TEST/hd15.txt"
run_test_exact "heredoc piped to tr" \
    "cat <<EOF | tr 'a-z' 'A-Z' > $HD15
lowercase
EOF" \
    "LOWERCASE" \
    "$HD15"

# =============================================================================
# SECTION: Heredoc - interaction with redirections
# =============================================================================
section "Heredoc: combined with redirections"

HD16="$TMPDIR_TEST/hd16.txt"
run_test "heredoc to file then cat that file" \
    "cat <<EOF > $HD16
redirect_test
EOF
cat $HD16" \
    "redirect_test"

HD17="$TMPDIR_TEST/hd17.txt"
echo "existing" > "$HD17"
run_test_exact "heredoc with append >>" \
    "cat <<EOF >> $HD17
appended
EOF" \
    "existing
appended" \
    "$HD17"

# =============================================================================
# SECTION: Heredoc - special content
# =============================================================================
section "Heredoc: special content"

HD18="$TMPDIR_TEST/hd18.txt"
run_test_exact "heredoc preserves leading spaces" \
    "cat <<EOF > $HD18
  leading spaces
EOF" \
    "  leading spaces" \
    "$HD18"

HD19="$TMPDIR_TEST/hd19.txt"
run_test_exact "heredoc with special chars !@#%^&*()" \
    "cat <<EOF > $HD19
hello!@#%^&*()
EOF" \
    "hello!@#%^&*()" \
    "$HD19"

HD20="$TMPDIR_TEST/hd20.txt"
run_test_exact "heredoc: partial delimiter in content not matched" \
    "cat <<EOF > $HD20
EOFX is not the delimiter
EOF" \
    "EOFX is not the delimiter" \
    "$HD20"

# =============================================================================
# SECTION: Heredoc - multiple heredocs in sequence
# =============================================================================
section "Heredoc: multiple heredocs in sequence"

HD21="$TMPDIR_TEST/hd21.txt"
HD22="$TMPDIR_TEST/hd22.txt"
# Two heredoc commands one after the other
printf '%s\nexit\n' "cat <<EOF > $HD21
first
EOF
cat <<EOF > $HD22
second
EOF" | "$MINISHELL" >/dev/null 2>&1

actual21=$(cat "$HD21" 2>/dev/null)
actual22=$(cat "$HD22" 2>/dev/null)

if [ "$actual21" = "first" ]; then
    printf "${GREEN}[PASS]${RESET} heredoc: first of two sequential heredocs\n"
    PASS=$((PASS + 1))
else
    printf "${RED}[FAIL]${RESET} heredoc: first of two sequential heredocs\n"
    printf "       expected: |first|\n"
    printf "       actual:   |%s|\n" "$actual21"
    FAIL=$((FAIL + 1))
fi

if [ "$actual22" = "second" ]; then
    printf "${GREEN}[PASS]${RESET} heredoc: second of two sequential heredocs\n"
    PASS=$((PASS + 1))
else
    printf "${RED}[FAIL]${RESET} heredoc: second of two sequential heredocs\n"
    printf "       expected: |second|\n"
    printf "       actual:   |%s|\n" "$actual22"
    FAIL=$((FAIL + 1))
fi

# =============================================================================
# SECTION: Command execution
# =============================================================================
section "Command execution"

run_test "run external command ls" \
    "ls /" \
    "bin"

run_test "absolute path command /bin/echo" \
    "/bin/echo absolute_path_test" \
    "absolute_path_test"

run_test "command not found does not crash" \
    "nonexistent_command_xyz123" \
    ""

# =============================================================================
# SUMMARY
# =============================================================================
printf "\n${CYAN}================================${RESET}\n"
printf "${CYAN}Test Results:${RESET}\n"
printf "  ${GREEN}PASS: %d${RESET}\n" "$PASS"
printf "  ${RED}FAIL: %d${RESET}\n" "$FAIL"
TOTAL=$((PASS + FAIL))
printf "  Total: %d\n" "$TOTAL"
printf "${CYAN}================================${RESET}\n"

# Cleanup
rm -rf "$TMPDIR_TEST"

if [ "$FAIL" -gt 0 ]; then
    exit 1
fi
exit 0
