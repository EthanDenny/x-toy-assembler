Token r0() {
    Token t = Token();
    t.type = REGISTER;
    t.value = '0';
    return t;
}

Token mFF() {
    Token t = Token();
    t.type = MEMORY;
    t.value = "FF";
    return t;
}
