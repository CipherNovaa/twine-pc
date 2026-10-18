import ghidra.app.script.GhidraScript;
import ghidra.program.model.mem.MemoryBlock;

public class ListBlocks extends GhidraScript {
    @Override
    protected void run() throws Exception {
        for (MemoryBlock b : currentProgram.getMemory().getBlocks()) {
            println(String.format("BLOCK %-16s %s-%s init=%s overlay=%s", b.getName(), b.getStart(), b.getEnd(),
                b.isInitialized(), b.isOverlay()));
        }
    }
}
