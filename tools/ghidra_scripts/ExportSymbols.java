// Exports named functions inside the CODE block as splat symbol_addrs lines.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.mem.MemoryBlock;
import java.nio.file.Files;
import java.nio.file.Path;

public class ExportSymbols extends GhidraScript {
    @Override
    protected void run() throws Exception {
        String out = getScriptArgs()[0];
        MemoryBlock code = currentProgram.getMemory().getBlock("CODE");
        StringBuilder sb = new StringBuilder();
        int n = 0;
        for (Function f : currentProgram.getFunctionManager().getFunctions(code.getStart(), true)) {
            if (!code.contains(f.getEntryPoint())) break;
            String name = f.getName();
            if (name.startsWith("FUN_")) continue;
            name = name.replaceAll("[^A-Za-z0-9_]", "_");
            if (Character.isDigit(name.charAt(0))) name = "psyq_" + name;
            sb.append(String.format("%s = 0x%08X; // type:func%n", name, f.getEntryPoint().getOffset()));
            n++;
        }
        Files.writeString(Path.of(out), sb.toString());
        println("EXPORTED=" + n);
    }
}
