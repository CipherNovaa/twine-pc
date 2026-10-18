// Prints decompiled C for each function address/name passed as script args.
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;

public class DecompileFuncs extends GhidraScript {
    @Override
    protected void run() throws Exception {
        DecompInterface ifc = new DecompInterface();
        ifc.openProgram(currentProgram);
        for (String arg : getScriptArgs()) {
            Function f = getGlobalFunctions(arg).stream().findFirst().orElse(null);
            if (f == null) f = getFunctionContaining(toAddr(arg));
            if (f == null) {
                printerr("no function: " + arg);
                continue;
            }
            DecompileResults r = ifc.decompileFunction(f, 60, monitor);
            println("==== " + f.getName() + " @ " + f.getEntryPoint() + " ====\n"
                + (r.decompileCompleted() ? r.getDecompiledFunction().getC() : r.getErrorMessage()));
        }
        ifc.dispose();
    }
}
