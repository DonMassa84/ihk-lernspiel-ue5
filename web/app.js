// IHK Lernspiel — Web App
const STORAGE_KEY = 'ihk_lernspiel_save';

let state = {
    questions: [],
    filtered: [],
    currentIndex: 0,
    correctAnswers: 0,
    totalQuestions: 0,
    priorityFilter: 0,
    gameState: 'menu',
    saveGame: null
};

// SaveGame
function loadSaveGame() {
    try {
        const raw = localStorage.getItem(STORAGE_KEY);
        if (raw) return JSON.parse(raw);
    } catch (e) {}
    return { totalSessions: 0, bestScore: 0, averageScore: 0, sessions: [], questionProgress: {} };
}

function saveSaveGame(sg) {
    localStorage.setItem(STORAGE_KEY, JSON.stringify(sg));
}

function initSaveGame() {
    state.saveGame = loadSaveGame();
    updateStatsDisplay();
}

function updateStatsDisplay() {
    document.getElementById('stat-sessions').textContent = state.saveGame.totalSessions;
    document.getElementById('stat-best').textContent = state.saveGame.bestScore + '%';
    document.getElementById('stat-avg').textContent = Math.round(state.saveGame.averageScore) + '%';
}

// Screen management
function showScreen(id) {
    document.querySelectorAll('.screen').forEach(s => s.classList.remove('active'));
    document.getElementById(id).classList.add('active');
}

function returnToMenu() {
    state.gameState = 'menu';
    updateStatsDisplay();
    showScreen('main-menu');
}

// Game flow
function startGame(priorityFilter) {
    state.priorityFilter = priorityFilter === 'all' ? 0 : priorityFilter;
    state.questions = [...QUESTIONS];
    state.filtered = state.priorityFilter === 0
        ? [...state.questions]
        : state.questions.filter(q => q.priority === state.priorityFilter);
    // Shuffle
    for (let i = state.filtered.length - 1; i > 0; i--) {
        const j = Math.floor(Math.random() * (i + 1));
        [state.filtered[i], state.filtered[j]] = [state.filtered[j], state.filtered[i]];
    }
    state.currentIndex = 0;
    state.correctAnswers = 0;
    state.totalQuestions = state.filtered.length;
    state.gameState = 'playing';

    if (state.totalQuestions > 0) {
        showScreen('question-screen');
        showQuestion();
    } else {
        alert('Keine Fragen für diese Priorität verfügbar.');
    }
}

function showQuestion() {
    const q = state.filtered[state.currentIndex];
    document.getElementById('q-index').textContent = state.currentIndex + 1;
    document.getElementById('q-total').textContent = state.totalQuestions;
    document.getElementById('q-score').textContent = state.correctAnswers;
    document.getElementById('q-category').textContent = q.category;
    document.getElementById('q-priority').textContent = 'Priorität ' + q.priority;
    document.getElementById('q-text').textContent = q.question;

    const progress = state.totalQuestions > 0 ? (state.currentIndex / state.totalQuestions * 100) : 0;
    document.getElementById('progress-fill').style.width = progress + '%';

    const optionsDiv = document.getElementById('q-options');
    optionsDiv.innerHTML = '';
    q.options.forEach((opt, idx) => {
        const btn = document.createElement('button');
        btn.className = 'option-btn';
        btn.textContent = opt;
        btn.onclick = () => submitAnswer(idx);
        optionsDiv.appendChild(btn);
    });

    const feedback = document.getElementById('feedback');
    feedback.classList.add('hidden');
    feedback.innerHTML = '';
}

function submitAnswer(selectedIndex) {
    const q = state.filtered[state.currentIndex];
    const isCorrect = selectedIndex === q.correctIndex;

    if (isCorrect) state.correctAnswers++;

    // Disable options
    document.querySelectorAll('.option-btn').forEach((btn, idx) => {
        btn.disabled = true;
        if (idx === q.correctIndex) btn.classList.add('correct');
        else if (idx === selectedIndex && !isCorrect) btn.classList.add('wrong');
    });

    // Show feedback
    const feedback = document.getElementById('feedback');
    feedback.classList.remove('hidden');
    feedback.innerHTML = `
        <div class="feedback-${isCorrect ? 'correct' : 'wrong'}">
            <strong>${isCorrect ? '✓ Richtig!' : '✗ Falsch'}</strong>
            <p><strong>Modellantwort:</strong> ${q.modelAnswer}</p>
            <p><strong>Schlüsselbegriffe:</strong> ${q.keywords.join(', ')}</p>
        </div>
    `;

    // Save progress
    const sg = state.saveGame;
    if (!sg.questionProgress[q.id]) sg.questionProgress[q.id] = { asked: 0, correct: 0 };
    sg.questionProgress[q.id].asked++;
    if (isCorrect) sg.questionProgress[q.id].correct++;

    // Next question after delay
    setTimeout(() => {
        state.currentIndex++;
        if (state.currentIndex < state.totalQuestions) {
            showQuestion();
        } else {
            finishGame();
        }
    }, 4000);
}

function skipQuestion() {
    state.currentIndex++;
    if (state.currentIndex < state.totalQuestions) {
        showQuestion();
    } else {
        finishGame();
    }
}

function finishGame() {
    const percentage = state.totalQuestions > 0
        ? Math.round(state.correctAnswers * 100 / state.totalQuestions)
        : 0;

    // Update save game
    const sg = state.saveGame;
    sg.totalSessions++;
    sg.sessions.push({ date: new Date().toISOString(), percentage, correct: state.correctAnswers, total: state.totalQuestions, priority: state.priorityFilter });
    if (percentage > sg.bestScore) sg.bestScore = percentage;
    const total = sg.sessions.reduce((s, x) => s + x.percentage, 0);
    sg.averageScore = total / sg.sessions.length;
    saveSaveGame(sg);

    // Show results
    document.getElementById('result-percentage').textContent = percentage + '%';
    document.getElementById('result-correct').textContent = state.correctAnswers;
    document.getElementById('result-total').textContent = state.totalQuestions;

    let message = '';
    if (percentage >= 90) message = 'Ausgezeichnet — Gesprächsreif!';
    else if (percentage >= 70) message = 'Gut — Noch 1-2 Fragen wiederholen';
    else if (percentage >= 50) message = 'Ausbaufähig — Leitfaden noch mal lesen';
    else message = 'Nachholbedarf — Alle Dokumente durcharbeiten';
    document.getElementById('result-message').textContent = message;

    showScreen('results-screen');
}

function showStats() {
    const sg = state.saveGame;
    let html = '<div class="stats-detail">';
    html += `<p><strong>Sessions gesamt:</strong> ${sg.totalSessions}</p>`;
    html += `<p><strong>Beste Punktzahl:</strong> ${sg.bestScore}%</p>`;
    html += `<p><strong>Durchschnitt:</strong> ${Math.round(sg.averageScore)}%</p>`;
    html += '<h3>Fragen-Fortschritt</h3><table><tr><th>Frage</th><th>Kategorie</th><th>Gefragt</th><th>Richtig</th></tr>';
    QUESTIONS.forEach(q => {
        const p = sg.questionProgress[q.id] || { asked: 0, correct: 0 };
        html += `<tr><td>${q.id}</td><td>${q.category}</td><td>${p.asked}</td><td>${p.correct}</td></tr>`;
    });
    html += '</table>';
    if (sg.sessions.length > 0) {
        html += '<h3>Letzte Sessions</h3><ul>';
        sg.sessions.slice(-5).reverse().forEach(s => {
            html += `<li>${new Date(s.date).toLocaleDateString('de-DE')} — ${s.correct}/${s.total} (${Math.round(s.correct/s.total*100)}%)</li>`;
        });
        html += '</ul>';
    }
    html += '</div>';
    document.getElementById('stats-content').innerHTML = html;
    showScreen('stats-screen');
}

function resetProgress() {
    if (confirm('Fortschritt wirklich zurücksetzen?')) {
        localStorage.removeItem(STORAGE_KEY);
        state.saveGame = loadSaveGame();
        updateStatsDisplay();
        alert('Fortschritt zurückgesetzt.');
    }
}

// Init
document.addEventListener('DOMContentLoaded', () => {
    initSaveGame();
});
