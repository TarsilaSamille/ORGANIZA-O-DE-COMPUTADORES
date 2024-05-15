class AutoEncoder:
    def __init__(self, input_dim, encoding_dim, decoder_dim):
        self.input_dim = input_dim
        self.encoding_dim = encoding_dim
        self.decoder_dim = decoder_dim
        
        # Pesos aleatórios para o codificador e decodificador
        self.encoder_weights = np.random.randn(encoding_dim, input_dim)
        self.decoder_weights = np.random.randn(decoder_dim, encoding_dim)
        
    def encode(self, x):
        encoded = np.dot(x, self.encoder_weights)
        return encoded
    
    def decode(self, encoded):
        decoded = np.dot(encoded, self.decoder_weights)
        return decoded
    
    def fit(self, inputs, epochs=10, learning_rate=0.01):
        for epoch in range(epochs):
            for i, x in enumerate(inputs):
                encoded = self.encode(x)
                decoded = self.decode(encoded)
                
                # Calcula a diferença entre o texto original e o texto decodificado
                loss = np.mean((x - decoded)**2)
                
                # Atualiza os pesos do decodificador
                self.decoder_weights -= learning_rate * (decoded - x) @ encoded.T
                
                # Atualiza os pesos do codificador
                self.encoder_weights -= learning_rate * (encoded - x) @ x.T
# Exemplo de uso
input_dim = 100  # Dimensão do espaço de entrada
encoding_dim = 50  # Dimensão do espaço de codificação
decoder_dim = 100  # Dimensão do espaço de decodificação

autoencoder = AutoEncoder(input_dim, encoding_dim, decoder_dim)

# Gerar alguns dados de exemplo
inputs = np.random.rand(100, input_dim)

# Treinar o autoencoder
autoencoder.fit(inputs, epochs=1000, learning_rate=0.001)

# Decodificar um exemplo
decoded_example = autoencoder.decode(autoencoder.encode(inputs[0]))
print(decoded_example)
