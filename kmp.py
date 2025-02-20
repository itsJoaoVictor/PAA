def kmp_prefix_function(pattern):
    m = len(pattern)
    lps = [0] * m  # Lista para armazenar a função de prefixo (longest prefix suffix)
    length = 0  # Comprimento do maior prefixo que também é sufixo
    i = 1
    
    while i < m:
        if pattern[i] == pattern[length]:
            length += 1
            lps[i] = length
            i += 1
        else:
            if length != 0:
                length = lps[length - 1]  # Reduzimos o comprimento do prefixo
            else:
                lps[i] = 0
                i += 1
    return lps

def kmp_search(text, pattern):
    n = len(text)
    m = len(pattern)
    lps = kmp_prefix_function(pattern)  # Obtém a tabela de prefixo
    i = 0  # Índice para o texto
    j = 0  # Índice para o padrão
    comparisons = []  # Para armazenar as comparações feitas
    
    while i < n:
        comparisons.append(f"Comparando {text[i]} e {pattern[j]}")  # Registro da comparação
        
        if pattern[j] == text[i]:
            i += 1
            j += 1
        
        if j == m:  # Encontrou uma ocorrência
            comparisons.append(f"Ocorrência encontrada no índice {i - j}")
            j = lps[j - 1]  # Usar o prefixo para continuar a busca
        
        elif i < n and pattern[j] != text[i]:  # Se falhou na comparação
            if j != 0:
                j = lps[j - 1]  # Usar a tabela para pular comparações
            else:
                i += 1
    
    # Caso não encontre nenhuma ocorrência
    if j != m:
        comparisons.append("Nenhuma ocorrência encontrada.")
    
    return comparisons

# Definindo a sequência de DNA e o gene
dna = "AAAATTTCGTTAAATTTGAACATAGGGATA"
gene = "TTTTTTGGGG"

# Realizando a busca
comparisons = kmp_search(dna, gene)

# Exibindo as comparações realizadas
for cmp in comparisons:
    print(cmp)
