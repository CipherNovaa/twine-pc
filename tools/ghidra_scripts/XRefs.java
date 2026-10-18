// Prints references to each symbol name or address passed as an argument.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.Symbol;

public class XRefs extends GhidraScript {
    @Override
    protected void run() throws Exception {
        for (String arg : getScriptArgs()) {
            Address target = null;
            for (Symbol s : currentProgram.getSymbolTable().getSymbols(arg)) {
                target = s.getAddress();
            }
            if (target == null) target = toAddr(arg);
            println("== refs to " + arg + " @ " + target);
            for (Reference r : getReferencesTo(target)) {
                Function f = getFunctionContaining(r.getFromAddress());
                println("  " + r.getFromAddress() + " " + r.getReferenceType() + " in " + (f == null ? "?" : f.getName()));
            }
        }
    }
}
