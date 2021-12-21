using System;
using System.ComponentModel;
using System.Diagnostics;

namespace processes {
    public static class ProcessExtensions {
        private static string FindIndexedProcessName(int pid) {
            var processName = Process.GetProcessById(pid).ProcessName;
            var processesByName = Process.GetProcessesByName(processName);
            string processIndexedName = null;
    
            for (int i = 0; i < processesByName.Length; ++i) {
                processIndexedName = i == 0 ? processName : processName + "#" + i;
                var processId = new PerformanceCounter("Process", "ID Process", processIndexedName);
                if ((int) processId.NextValue() == pid) {
                    return processIndexedName;
                }
            }
    
            return processIndexedName;
        }
    
        private static Process FindPidFromIndexedProcessName(string indexedProcessName) {
            var parentId = new PerformanceCounter("Process", "Creating Process ID", indexedProcessName);
            return Process.GetProcessById((int) parentId.NextValue());
        }
    
        public static Process Parent(this Process process) {
            return FindPidFromIndexedProcessName(FindIndexedProcessName(process.Id));
        }
    }

    internal static class Program {
        public static void PrintPadLeft(string str, int leftPadding) {
            Console.WriteLine(str.PadLeft(leftPadding + str.Length, ' '));
        }
        
        public static void PrintThreadInfo(ProcessThread thread, int leftPadding) {
            string threadInfo = $"Thread: ID: {thread.Id}, priority: {thread.BasePriority}";
            PrintPadLeft(threadInfo, leftPadding);
        }
        
        public static void PrintModuleInfo(ProcessModule module, int leftPadding) {
            string moduleInfo = $"Module: Name: {module.ModuleName}, size: {module.ModuleMemorySize}," +
                         $" file path: {module.FileName}";
            PrintPadLeft(moduleInfo, leftPadding);
        }

        public static void PrintProcessInfo(Process process, int leftPadding) {
            var threads = process.Threads;
            ProcessModuleCollection modules = null;
            try {
                modules = process.Modules;
            }
            catch (Win32Exception) {}

            string parentPidStr = "None";
            try {
                parentPidStr = process.Parent().Id.ToString();
            }
            catch (ArgumentException) {}
            
            string processInfo = $"Process name: {process.ProcessName}, PID: {process.Id}," +
                                 $" Parent PID: {parentPidStr}, Priority: {process.BasePriority}";
            string threadsInfo = threads.Count == 0 ? "Threads: None" : $"Threads ({threads.Count}):";
            string modulesInfo = modules == null ? "Modules: None" : $"Modules ({modules.Count}):";
                
            PrintPadLeft(processInfo, leftPadding);
            PrintPadLeft(threadsInfo, leftPadding);
            foreach (ProcessThread thread in threads) {
                PrintThreadInfo(thread, leftPadding + 4);
            }
            
            PrintPadLeft(modulesInfo, leftPadding);
            if (modules == null) return;
            foreach (ProcessModule module in modules) {
                PrintModuleInfo(module, leftPadding + 4);
            }
        }
        
        public static void Main(string[] args) {
            var processes = Process.GetProcesses();
            Console.WriteLine($"Processes count: {processes.Length}");
            foreach (var process in processes) {
                try {
                    PrintProcessInfo(process, 0);
                }
                catch (InvalidOperationException) {
                    Console.WriteLine("Process was finished!");
                }
                Console.WriteLine();
            }
        }
    }
}