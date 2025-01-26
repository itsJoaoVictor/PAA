# Caminhos dos arquivos
arquivo1 = r"C:\Users\Pichau\Codigos\Projetos UFS\PAA\output\quicksort.output"
arquivo2 = r"C:\Users\Pichau\Codigos\Projetos UFS\PAA\quicksortoutputesperado.output"

# Abrir e ler os arquivos
with open(arquivo1, 'r') as f1, open(arquivo2, 'r') as f2:
    conteudo1 = f1.readlines()
    conteudo2 = f2.readlines()

# Comparar os conteúdos
if conteudo1 == conteudo2:
    print("✅ Os arquivos são iguais!")
else:
    print("❌ Os arquivos são diferentes!\n")
    print("Diferenças encontradas:")
    print("-" * 50)

    # Comparando linha a linha
    for i, (linha1, linha2) in enumerate(zip(conteudo1, conteudo2)):
        if linha1.strip() == linha2.strip():
            print(f"✅ Linha {i}: As linhas são iguais.")
        else:
            print(f"❌ Linha {i}: As linhas são diferentes!")
            print(f"   Linha 1: {linha1.strip()}")
            print(f"   Linha 2: {linha2.strip()}")

    # Para o caso de arquivos com mais linhas
    if len(conteudo1) > len(conteudo2):
        print("\n⚠️ Linhas extras no arquivo gerado:")
        for linha in conteudo1[len(conteudo2):]:
            print(f"   {linha.strip()}")

    if len(conteudo2) > len(conteudo1):
        print("\n⚠️ Linhas extras no arquivo esperado:")
        for linha in conteudo2[len(conteudo1):]:
            print(f"   {linha.strip()}")
