// Writes "<entry> <size> <name>" for every function in the CODE block to <outdir>/<program>.funcs
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.mem.MemoryBlock;
import java.nio.file.Files;
import java.nio.file.Path;

public class ExportFunctions extends GhidraScript {
    @Override
    protected void run() throws Exception {
        MemoryBlock code = currentProgram.getMemory().getBlock("CODE");
        StringBuilder sb = new StringBuilder();
        for (Function f : currentProgram.getFunctionManager().getFunctions(code.getStart(), true)) {
            if (!code.contains(f.getEntryPoint())) break;
            long size = f.getBody().getMaxAddress().getOffset() - f.getEntryPoint().getOffset() + 1;
            sb.append(String.format("%08X %X %s%n", f.getEntryPoint().getOffset(), size, f.getName()));
        }
        Path out = Path.of(getScriptArgs()[0], currentProgram.getName() + ".funcs");
        Files.writeString(out, sb.toString());
        println("WROTE " + out);
    }
}
