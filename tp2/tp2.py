import tkinter as tk
from tkinter import messagebox
import subprocess
import sys

def run_c_program(matrix_size, num_threads):
    try:
        result = subprocess.run(
            [f'./{sys.argv[1]}', str(matrix_size), str(num_threads)], 
            capture_output=True, text=True, check=True
        )
        output = result.stdout
        messagebox.showinfo("Resultado", output)
    except subprocess.CalledProcessError as e:
        messagebox.showerror("Erro", f"Erro ao executar o programa C: {e}")

def on_submit():
    try:
        matrix_size = int(matrix_size_entry.get())
        num_threads = int(num_threads_entry.get())
        run_c_program(matrix_size, num_threads)
    except ValueError:
        messagebox.showerror("Erro", "Por favor, insira valores válidos.")

# Configuração da interface gráfica
root = tk.Tk()
root.title("Multiplicação de Matrizes")

tk.Label(root, text="Tamanho da Matriz:").pack(pady=5)
matrix_size_entry = tk.Entry(root)
matrix_size_entry.pack(pady=5)

tk.Label(root, text="Número de Threads:").pack(pady=5)
num_threads_entry = tk.Entry(root)
num_threads_entry.pack(pady=5)

submit_button = tk.Button(root, text="Calcular", command=on_submit)
submit_button.pack(pady=20)

root.mainloop()
