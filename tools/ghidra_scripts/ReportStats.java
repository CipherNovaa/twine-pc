import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import java.util.ArrayList;
import java.util.List;

public class ReportStats extends GhidraScript {
    @Override
    protected void run() throws Exception {
        FunctionManager fm = currentProgram.getFunctionManager();
        List<Function> funcs = new ArrayList<>();
        for (Function f : fm.getFunctions(true)) {
            funcs.add(f);
        }
        println("FUNCTION_COUNT=" + funcs.size());

        int defined = 0;
        for (Function f : funcs) {
            if (!f.isThunk()) defined++;
        }
        println("DEFINED_FUNC_COUNT=" + defined);

        println("INSTRUCTION_COUNT=" + currentProgram.getListing().getNumInstructions());
        println("MEMORY_SIZE=" + currentProgram.getMemory().getSize());
        println("DEFINED_SYMBOL_COUNT=" + currentProgram.getSymbolTable().getNumSymbols());

        println("--- first 25 functions ---");
        int shown = 0;
        for (Function f : funcs) {
            if (shown >= 25) break;
            println(f.getName() + " @ " + f.getEntryPoint() + "  size=" + f.getBody().getNumAddresses());
            shown++;
        }
    }
}
