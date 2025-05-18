import {
    Dispatch
} from 'react';


import {
    Text,
    TextInput,
    View
} from 'react-native';


interface SettingsProps {
    serverUrl: string;
    setServerUrl: Dispatch<string>;
}


export default function Settings({ serverUrl, setServerUrl }: SettingsProps) {
    return (
        <View>
            <View>
                <Text styles={{ textAlign: 'center' }}>
                    Settings
                </Text>
            </View>
            <TextInput
                onChangeText={setServerUrl}
                value={serverUrl}
                placeholder={'insert url'}
            />
        </View>
    )
};

