import threading
import random
import time
from datetime import datetime

# Definicion de Clases
class Trabajador:
    """representa un trabajador del equipo de extraccion"""
    def __init__(self, id):
        self.id = id
        self.resistencia = 100
        self.mochila = []  # Lista de cristales extraídos
        self.estado = "Activo"  # Activo, Agotado, Intoxicado
        self.historial_zonas = []
        self.historial_cristales = []
        self.rondas_trabajadas = 0
        self.rondas_descanso = 0
        self.lock = threading.Lock()  # Lock individual del trabajador
    
    def extraer_cristales(self, cantidad):
        """añade cristales a la mochila del trabajador"""
        with self.lock:
            self.mochila.extend([1] * cantidad)  # Cada cristal representado como 1
            self.historial_cristales.append(cantidad)
    
    def reducir_resistencia(self, cantidad):
        """reduce la resistencia del trabajador"""
        with self.lock:
            self.resistencia -= cantidad
            self._actualizar_estado()
    
    def _actualizar_estado(self):
        """actualiza el estado segun la resistencia"""
        if self.resistencia <= 0:
            self.estado = "Intoxicado"
        elif self.resistencia <= 20:
            self.estado = "Agotado"
        else:
            self.estado = "Activo"
    
    def descansar(self):
        """recupera resistencia al descansar"""
        with self.lock:
            self.resistencia += 30
            if self.resistencia > 100:
                self.resistencia = 100
            self._actualizar_estado()
    
    def dar_cristales(self, cantidad):
        """da cristales a otro trabajador maximo 3"""
        with self.lock:
            cantidad = min(cantidad, len(self.mochila), 3)
            for _ in range(cantidad):
                if self.mochila:
                    self.mochila.pop()
            return cantidad
    
    def recibir_ayuda(self, cristales):
        """recibe cristales y recupera resistencia"""
        with self.lock:
            self.resistencia += 15
            if self.resistencia > 100:
                self.resistencia = 100
            self._actualizar_estado()


class Zona:
    """representa una zona de extraccion de cristales"""
    def __init__(self, id, cristales):
        self.id = id
        self.cristales = cristales
        self.toxicidad = random.randint(10, 50)
        self.capacidad_maxima = 8
        self.semaforo = threading.Semaphore(8)  # maximo 8 trabajadores
        self.lock = threading.Lock()  # para proteger el inventario
        self.trabajadores_presentes = []
        self.lock_trabajadores = threading.Lock()
    
    def entrar(self, trabajador_id):
        """intenta entrar a la zona"""
        if self.semaforo.acquire(blocking=False):
            with self.lock_trabajadores:
                self.trabajadores_presentes.append(trabajador_id)
            return True
        return False
    
    def salir(self, trabajador_id):
        """sale de la zona"""
        with self.lock_trabajadores:
            if trabajador_id in self.trabajadores_presentes:
                self.trabajadores_presentes.remove(trabajador_id)
        self.semaforo.release()
    
    def extraer(self, cantidad):
        """extrae cristales de la zona"""
        with self.lock:
            cantidad_extraida = min(cantidad, self.cristales)
            self.cristales -= cantidad_extraida
            return cantidad_extraida
    
    def get_num_trabajadores(self):
        """retorna el numero de trabajadores en la zona"""
        with self.lock_trabajadores:
            return len(self.trabajadores_presentes)


# Sistema de simulacion
class SistemaExtraccion:
    """controla toda la simulacion de extraccion"""
    def __init__(self):
        self.trabajadores = []
        self.zonas = []
        self.ronda_actual = 0
        self.barrier = None
        self.logs = {
            'inicializacion': [],
            'rondas': {},
            'intercambios': [],
            'final': []
        }
        self.lock_logs = threading.Lock()
        self.lock_global = threading.Lock()
        
    def inicializar(self):
        """inicializa trabajadores y zonas"""
        print("Inicializando sistema de extracción...")
        
        # Crear 64 trabajadores
        for i in range(64):
            self.trabajadores.append(Trabajador(i))
        
        # Crear 16 zonas con cristales (total 256)
        cristales_restantes = 256
        for i in range(16):
            if i == 15:  # Última zona recibe los cristales restantes
                cristales = cristales_restantes
            else:
                cristales = random.randint(10, 20)
                cristales_restantes -= cristales
            self.zonas.append(Zona(i, cristales))
        
        # Ajustar si es necesario
        diferencia = 256 - sum(z.cristales for z in self.zonas)
        if diferencia != 0:
            self.zonas[0].cristales += diferencia
        
        # Registrar inicialización
        self._log_inicializacion()
        print("Sistema inicializado correctamente.")
    
    def _log_inicializacion(self):
        """registra la inicializacion en el archivo"""
        with self.lock_logs:
            self.logs['inicializacion'].append("=== INICIALIZACIÓN DEL SISTEMA ===\n")
            self.logs['inicializacion'].append(f"Fecha: {datetime.now()}\n\n")
            self.logs['inicializacion'].append("--- TRABAJADORES ---\n")
            for t in self.trabajadores:
                self.logs['inicializacion'].append(
                    f"Trabajador {t.id}: Resistencia={t.resistencia}, "
                    f"Capacidad Mochila=10, Estado={t.estado}\n"
                )
            
            self.logs['inicializacion'].append("\n--- ZONAS DE EXTRACCIÓN ---\n")
            total_cristales = 0
            for z in self.zonas:
                total_cristales += z.cristales
                self.logs['inicializacion'].append(
                    f"Zona {z.id}: Cristales={z.cristales}, "
                    f"Toxicidad={z.toxicidad}, Capacidad={z.capacidad_maxima}\n"
                )
            self.logs['inicializacion'].append(f"\nTotal de cristales: {total_cristales}\n")
    
    def ejecutar_simulacion(self):
        """ejecuta las 8 rondas de simulacion"""
        for ronda in range(8):
            self.ronda_actual = ronda
            self.logs['rondas'][ronda] = []
            
            print(f"\n{'='*50}")
            texto_ronda = f"RONDA {ronda + 1}/8"
            print(texto_ronda.center(50))
            print(f"{'='*50}")
            
            # Crear barrera para sincronizar trabajadores
            trabajadores_activos = [t for t in self.trabajadores if t.estado != "Intoxicado"]
            if not trabajadores_activos:
                print("Todos los trabajadores están intoxicados. Fin de la simulación.")
                break
            
            self.barrier = threading.Barrier(len(trabajadores_activos) + 1)  # +1 para el hilo principal
            
            # Crear y lanzar hebras
            threads = []
            for t in trabajadores_activos:
                thread = threading.Thread(target=self.trabajador_trabajar, args=(t,))
                threads.append(thread)
                thread.start()
            
            # Esperar que todos estén listos
            self.barrier.wait()
            
            # Esperar a que todos terminen
            for thread in threads:
                thread.join()
            
            # Gestión post-ronda
            self._gestionar_estados_post_ronda()
            self._intentar_intercambios()
            self._log_ronda()
            
            print(f"Ronda {ronda + 1} completada.")
            time.sleep(0.5)  # Pequeña pausa entre rondas
    
    def trabajador_trabajar(self, trabajador):
        """logica de trabajo de cada trabajador en una ronda"""
        # Sincronizar inicio de ronda
        self.barrier.wait()
        
        # Si está agotado, descansa
        if trabajador.estado == "Agotado":
            if trabajador.rondas_descanso > 0:
                trabajador.rondas_descanso -= 1
                return
            else:
                trabajador.descansar()
                trabajador.rondas_descanso = 0
                with self.lock_logs:
                    self.logs['rondas'][self.ronda_actual].append(
                        f"Trabajador {trabajador.id} se recuperó en el campamento "
                        f"(Resistencia: {trabajador.resistencia})\n"
                    )
        
        # Si está intoxicado, no hace nada
        if trabajador.estado == "Intoxicado":
            return
        
        # Si la mochila está llena, no puede extraer más
        if len(trabajador.mochila) >= 10:
            with self.lock_logs:
                self.logs['rondas'][self.ronda_actual].append(
                    f"Trabajador {trabajador.id} tiene la mochila llena\n"
                )
            return
        
        # Elegir zona (prioridad a zonas con más cristales si tiene pocos cristales)
        zona = self._elegir_zona(trabajador)
        
        if zona and zona.entrar(trabajador.id):
            try:
                trabajador.historial_zonas.append(zona.id)
                trabajador.rondas_trabajadas += 1
                
                # Intentar extraer cristales (1-3 aleatorio)
                cantidad_objetivo = random.randint(1, 3)
                cantidad_objetivo = min(cantidad_objetivo, 10 - len(trabajador.mochila))
                
                # Verificar competencia (70% éxito, 30% conflicto)
                exito = random.random() < 0.7
                
                if exito:
                    cantidad_extraida = zona.extraer(cantidad_objetivo)
                    if cantidad_extraida > 0:
                        trabajador.extraer_cristales(cantidad_extraida)
                        
                        # Calcular costo de resistencia
                        if cantidad_extraida == 1:
                            costo = 5
                        elif cantidad_extraida == 2:
                            costo = 12
                        else:
                            costo = 20
                        
                        trabajador.reducir_resistencia(costo)
                        
                        with self.lock_logs:
                            self.logs['rondas'][self.ronda_actual].append(
                                f"Trabajador {trabajador.id} extrajo {cantidad_extraida} cristales "
                                f"de Zona {zona.id} (Resistencia: {trabajador.resistencia}, "
                                f"Estado: {trabajador.estado})\n"
                            )
                else:
                    # Conflicto
                    trabajador.reducir_resistencia(10)
                    with self.lock_logs:
                        self.logs['rondas'][self.ronda_actual].append(
                            f"¡CONFLICTO! Trabajador {trabajador.id} en Zona {zona.id} "
                            f"(-10 resistencia, Resistencia: {trabajador.resistencia})\n"
                        )
                
                # Aplicar toxicidad de la zona
                num_trabajadores = zona.get_num_trabajadores()
                if num_trabajadores > 0:
                    toxicidad_individual = zona.toxicidad / num_trabajadores
                    trabajador.reducir_resistencia(int(toxicidad_individual))
                
            finally:
                zona.salir(trabajador.id)
        else:
            with self.lock_logs:
                self.logs['rondas'][self.ronda_actual].append(
                    f"Trabajador {trabajador.id} no pudo entrar a ninguna zona\n"
                )
    
    def _elegir_zona(self, trabajador):
        """elige una zona para el trabajador con prioridad si tiene pocos cristales"""
        # Si tiene pocos cristales, priorizar zonas con más recursos
        if len(trabajador.mochila) < 5:
            zonas_ordenadas = sorted(self.zonas, key=lambda z: z.cristales, reverse=True)
        else:
            zonas_ordenadas = self.zonas.copy()
            random.shuffle(zonas_ordenadas)
        
        # Intentar entrar a alguna zona
        for zona in zonas_ordenadas:
            if zona.cristales > 0:
                return zona
        
        return None
    
    def _gestionar_estados_post_ronda(self):
        """gestiona los estados de los trabajadores despues de cada ronda"""
        for t in self.trabajadores:
            if t.estado == "Agotado" and t.rondas_descanso == 0:
                t.rondas_descanso = 1
    
    def _intentar_intercambios(self):
        """intenta intercambios de cristales entre trabajadores"""
        trabajadores_activos = [t for t in self.trabajadores if t.estado == "Activo"]
        trabajadores_agotados = [t for t in self.trabajadores if t.estado == "Agotado"]
        
        for activo in trabajadores_activos:
            if len(activo.mochila) > 3 and trabajadores_agotados:
                agotado = random.choice(trabajadores_agotados)
                cantidad = activo.dar_cristales(3)
                if cantidad > 0:
                    agotado.recibir_ayuda(cantidad)
                    with self.lock_logs:
                        self.logs['intercambios'].append(
                            f"Ronda {self.ronda_actual + 1}: Trabajador {activo.id} dio "
                            f"{cantidad} cristales a Trabajador {agotado.id} "
                            f"(Resistencia agotado: {agotado.resistencia})\n"
                        )
                    trabajadores_agotados.remove(agotado)
                    if not trabajadores_agotados:
                        break
    
    def _log_ronda(self):
        """complementa el log de la ronda con resumen"""
        with self.lock_logs:
            self.logs['rondas'][self.ronda_actual].insert(
                0, f"\n=== RONDA {self.ronda_actual + 1} ===\n\n"
            )
            
            # Resumen de estados
            activos = sum(1 for t in self.trabajadores if t.estado == "Activo")
            agotados = sum(1 for t in self.trabajadores if t.estado == "Agotado")
            intoxicados = sum(1 for t in self.trabajadores if t.estado == "Intoxicado")
            
            self.logs['rondas'][self.ronda_actual].append(
                f"\n--- RESUMEN DE ESTADOS ---\n"
                f"Activos: {activos}\n"
                f"Agotados: {agotados}\n"
                f"Intoxicados: {intoxicados}\n"
            )
    
    def generar_reporte_final(self):
        """genera el reporte final de la extraccion"""
        print("\n" + "="*50)
        print("GENERANDO REPORTE FINAL")
        print("="*50)
        
        total_cristales = sum(len(t.mochila) for t in self.trabajadores)
        trabajadores_vivos = sum(1 for t in self.trabajadores if t.estado != "Intoxicado")
        eficiencia = (total_cristales * trabajadores_vivos) / 256
        
        with self.lock_logs:
            self.logs['final'].append("=== EXTRACCIÓN FINAL ===\n\n")
            self.logs['final'].append(f"Total de cristales extraídos: {total_cristales}\n\n")
            
            self.logs['final'].append("--- CRISTALES POR TRABAJADOR ---\n")
            for t in self.trabajadores:
                self.logs['final'].append(
                    f"Trabajador {t.id}: {len(t.mochila)} cristales, "
                    f"Estado: {t.estado}, Resistencia: {t.resistencia}\n"
                )
            
            intoxicados = [t for t in self.trabajadores if t.estado == "Intoxicado"]
            self.logs['final'].append(f"\n--- TRABAJADORES EVACUADOS ---\n")
            self.logs['final'].append(f"Total evacuados: {len(intoxicados)}\n")
            for t in intoxicados:
                self.logs['final'].append(f"Trabajador {t.id}\n")
            
            self.logs['final'].append(f"\n--- EFICIENCIA DEL EQUIPO ---\n")
            self.logs['final'].append(f"Eficiencia: {eficiencia:.2f}\n")
            self.logs['final'].append(f"Trabajadores sobrevivientes: {trabajadores_vivos}/64\n")
            self.logs['final'].append(f"Cristales extraídos: {total_cristales}/256\n")
        
        print(f"\nTotal de cristales extraídos: {total_cristales}")
        print(f"Trabajadores sobrevivientes: {trabajadores_vivos}/64")
        print(f"Eficiencia del equipo: {eficiencia:.2f}")
    
    def guardar_logs(self):
        """guarda todos los logs en archivos"""
        print("\nGuardando archivos de log...")
        
        # inicializacion.txt
        with open('inicializacion.txt', 'w', encoding='utf-8') as f:
            f.writelines(self.logs['inicializacion'])
        
        # ronda_X.txt
        for ronda, logs in self.logs['rondas'].items():
            with open(f'ronda_{ronda + 1}.txt', 'w', encoding='utf-8') as f:
                f.writelines(logs)
        
        # intercambios.txt
        with open('intercambios.txt', 'w', encoding='utf-8') as f:
            f.write("=== INTERCAMBIOS DE CRISTALES ===\n\n")
            if self.logs['intercambios']:
                f.writelines(self.logs['intercambios'])
            else:
                f.write("No hubo intercambios durante la simulación.\n")
        
        # extraccion_final.txt
        with open('extraccion_final.txt', 'w', encoding='utf-8') as f:
            f.writelines(self.logs['final'])
        
        print("Archivos guardados correctamente.")


# Programa principal

def main():
    """funcion principal que ejecuta todo el sistema"""
    print("\n" + "="*70)
    print(" " * 15 + "SISTEMA DE EXTRACCIÓN DE CRISTALES")
    print("="*70 + "\n")
    
    sistema = SistemaExtraccion()
    sistema.inicializar()
    sistema.ejecutar_simulacion()
    sistema.generar_reporte_final()
    sistema.guardar_logs()
    
    print("\n" + "="*70)
    print(" " * 20 + "SIMULACIÓN COMPLETADA")
    print("="*70 + "\n")


if __name__ == "__main__":
    main()