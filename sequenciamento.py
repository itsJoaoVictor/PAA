import difflib

def comparar_arquivos(arquivo1, arquivo2):
    with open(arquivo1, 'r', encoding='utf-8') as f1, open(arquivo2, 'r', encoding='utf-8') as f2:
        conteudo1 = f1.readlines()
        conteudo2 = f2.readlines()

    # Garante que os arquivos tenham o mesmo número de linhas para comparação
    max_linhas = max(len(conteudo1), len(conteudo2))
    conteudo1 += ['\n'] * (max_linhas - len(conteudo1))
    conteudo2 += ['\n'] * (max_linhas - len(conteudo2))

    encontrou_diferenca = False

    for i, (linha1, linha2) in enumerate(zip(conteudo1, conteudo2), start=1):
        if linha1.strip() != linha2.strip():
            encontrou_diferenca = True
            print(f"Linha {i}:")
            print(f"  Sua saída     : {linha1.strip()}")
            print(f"  Saída desejada: {linha2.strip()}")
            print("-" * 80)

    if not encontrou_diferenca:
        print("Os arquivos são idênticos.")

if __name__ == "__main__":
    arquivo1 = r"C:\Users\Pichau\Codigos\Projetos UFS\PAA\output\sequenciamento.output"
    arquivo2 = r"C:\Users\Pichau\Codigos\Projetos UFS\PAA\sequenciamentoDesejado.output"
    comparar_arquivos(arquivo1, arquivo2)
